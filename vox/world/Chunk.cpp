
#include "vox/world/Chunk.h"

#include "vox/world/ChunkSize.h"
#include "vox/world/data/ChunkDataTranslator.h"

void vox::world::Chunk::setNeighbor(Chunk* neighbor, const Side& side)
{
	if (side.id == Side::UNKNOWN.id)
		return;
	if (neighbor == nullptr && m_neighbors[side.id] != nullptr)
		m_neighbors[side.id]->m_neighbors[side.opposite.id] = nullptr;
	m_neighbors[side.id] = neighbor;
	if (neighbor != nullptr)
		neighbor->m_neighbors[side.opposite.id] = this;
}

vox::data::BlockData vox::world::Chunk::getBlock(const glm::uvec3& pos) const
{
	return m_data->getBlock(pos);
}
void vox::world::Chunk::acceptQuery(data::BlockReadQuery& query) const
{
	m_data->acceptQuery(query);
}
void vox::world::Chunk::acceptQuery(data::BlockWriteQuery& query)
{
	data::ChunkDataTranslator translator;

	if (m_data == nullptr)
		m_data = &m_dataFlat;
	else if (m_data == &m_dataRLE)
	{
		m_data = &m_dataFlat;
		m_dataFlat = translator.toFlat(m_dataRLE);
		m_dataRLE.forget();
	}

	m_data->acceptQuery(query);

	m_data = &m_dataRLE;
	m_dataRLE = translator.toRLE(m_dataFlat);
	m_dataFlat.forget();
}

vox::data::BlockRegion vox::world::Chunk::getMeshingData() const
{
	const unsigned int SIZE = static_cast<unsigned int>(chunk::SIZE);
	const unsigned int SIZE_MINUS_ONE = static_cast<unsigned int>(chunk::SIZE_MINUS_ONE);

	data::BlockRegion region{ glm::ivec3{ -1 }, glm::ivec3{ chunk::SIZE + 2 } };

	m_data->fillRegion(region, glm::uvec3{}, glm::ivec3{}, glm::uvec3{ SIZE });
	if (const auto neightbor = getNeighbor(Side::NORTH))
		neightbor->m_data->fillRegion(region, glm::ivec3{}, glm::ivec3{ SIZE, 0, 0 }, glm::uvec3{ 1, SIZE, SIZE });
	if (const auto neightbor = getNeighbor(Side::SOUTH))
		neightbor->m_data->fillRegion(region, glm::ivec3{ SIZE_MINUS_ONE, 0, 0 }, glm::ivec3{ -chunk::SIZE, 0, 0 }, glm::uvec3{ 1, SIZE, SIZE });
	if (const auto neightbor = getNeighbor(Side::WEST))
		neightbor->m_data->fillRegion(region, glm::ivec3{}, glm::ivec3{ 0, SIZE, 0 }, glm::uvec3{ SIZE, 1, SIZE });
	if (const auto neightbor = getNeighbor(Side::EAST))
		neightbor->m_data->fillRegion(region, glm::ivec3{ 0, SIZE_MINUS_ONE, 0 }, glm::ivec3{ 0, -chunk::SIZE, 0 }, glm::uvec3{ SIZE, 1, SIZE });
	if (const auto neightbor = getNeighbor(Side::TOP))
		neightbor->m_data->fillRegion(region, glm::ivec3{}, glm::ivec3{ 0, 0, SIZE }, glm::uvec3{ SIZE, SIZE, 1 });
	if (const auto neightbor = getNeighbor(Side::BOTTOM))
		neightbor->m_data->fillRegion(region, glm::ivec3{ 0, 0, SIZE_MINUS_ONE }, glm::ivec3{ 0, 0, -chunk::SIZE }, glm::uvec3{ SIZE, SIZE, 1 });

	return region;
}
vox::data::ChunkDataRLE vox::world::Chunk::getStoringData() const
{
	if (m_data != &m_dataFlat)
		return m_dataRLE;
	data::ChunkDataTranslator translator;
	return translator.toRLE(m_dataFlat);
}
void vox::world::Chunk::injectChunkData(data::ChunkDataRLE&& data)
{
	m_dataFlat.forget();
	m_dataRLE = std::move(data);
	m_data = &m_dataRLE;
}

unsigned int vox::world::Chunk::memusage() const
{
	return m_dataFlat.memusage() + m_dataRLE.memusage();
}
bool vox::world::Chunk::empty() const
{
	return m_dataFlat.empty() && m_dataRLE.empty();
}

