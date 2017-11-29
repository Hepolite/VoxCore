
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
void vox::world::Chunk::setNearest(Chunk* above, Chunk* below)
{
	if (above == nullptr && m_nearestAbove != nullptr)
		m_nearestAbove->m_nearestBelow = m_nearestBelow;
	if (below == nullptr && m_nearestBelow != nullptr)
		m_nearestBelow->m_nearestAbove = m_nearestAbove;

	m_nearestAbove = above;
	m_nearestBelow = below;

	if (above != nullptr)
		above->m_nearestBelow = this;
	if (below != nullptr)
		below->m_nearestAbove = this;
}

vox::data::BlockData vox::world::Chunk::getBlock(const glm::uvec3& pos) const
{
	if (!m_dataRLE.empty())
		return m_dataRLE.getBlock(pos);
	return m_dataFlat.getBlock(pos);
}
void vox::world::Chunk::acceptReadQuery(data::BlockQuery& query) const
{
	if (!m_dataRLE.empty())
		m_dataRLE.acceptReadQuery(query);
	else
		m_dataFlat.acceptReadQuery(query);
}
void vox::world::Chunk::acceptWriteQuery(data::BlockQuery& query)
{
	expand();
	m_dataFlat.acceptWriteQuery(query);
	//contract();
}

vox::data::ChunkDataFlat & vox::world::Chunk::getActualData()
{
	expand();
	return m_dataFlat;
}
vox::data::BlockRegion vox::world::Chunk::getMeshingData() const
{
	const unsigned int SIZE = static_cast<unsigned int>(chunk::SIZE);
	const unsigned int SIZE_MINUS_ONE = static_cast<unsigned int>(chunk::SIZE_MINUS_ONE);

	data::BlockRegion region{ glm::ivec3{ -1 }, glm::ivec3{ chunk::SIZE + 2 } };

	getMeshingData(region, glm::uvec3{}, glm::ivec3{}, glm::uvec3{ SIZE });
	if (const auto neightbor = getNeighbor(Side::NORTH))
		neightbor->getMeshingData(region, glm::ivec3{}, glm::ivec3{ SIZE, 0, 0 }, glm::uvec3{ 1, SIZE, SIZE });
	if (const auto neightbor = getNeighbor(Side::SOUTH))
		neightbor->getMeshingData(region, glm::ivec3{ SIZE_MINUS_ONE, 0, 0 }, glm::ivec3{ -chunk::SIZE, 0, 0 }, glm::uvec3{ 1, SIZE, SIZE });
	if (const auto neightbor = getNeighbor(Side::WEST))
		neightbor->getMeshingData(region, glm::ivec3{}, glm::ivec3{ 0, SIZE, 0 }, glm::uvec3{ SIZE, 1, SIZE });
	if (const auto neightbor = getNeighbor(Side::EAST))
		neightbor->getMeshingData(region, glm::ivec3{ 0, SIZE_MINUS_ONE, 0 }, glm::ivec3{ 0, -chunk::SIZE, 0 }, glm::uvec3{ SIZE, 1, SIZE });
	if (const auto neightbor = getNeighbor(Side::TOP))
		neightbor->getMeshingData(region, glm::ivec3{}, glm::ivec3{ 0, 0, SIZE }, glm::uvec3{ SIZE, SIZE, 1 });
	if (const auto neightbor = getNeighbor(Side::BOTTOM))
		neightbor->getMeshingData(region, glm::ivec3{ 0, 0, SIZE_MINUS_ONE }, glm::ivec3{ 0, 0, -chunk::SIZE }, glm::uvec3{ SIZE, SIZE, 1 });

	return region;
}
void vox::world::Chunk::getMeshingData(data::BlockRegion& region, const glm::uvec3& position, const glm::uvec3& offset, const glm::uvec3& size) const
{
	if (!m_dataRLE.empty())
		m_dataRLE.acceptRegionQuery(region, position, offset, size);
	else
		m_dataFlat.acceptRegionQuery(region, position, offset, size);
}
vox::data::ChunkDataRLE vox::world::Chunk::getStoringData() const
{
	if (!m_dataRLE.empty())
		return m_dataRLE;
	data::ChunkDataTranslator translator;
	return translator.toRLE(m_dataFlat);
}
void vox::world::Chunk::injectChunkData(data::ChunkDataRLE&& data)
{
	m_dataFlat.forget();
	m_dataRLE = std::move(data);
}

void vox::world::Chunk::expand()
{
	if (!m_dataFlat.empty())
		return;
	m_dataFlat = data::ChunkDataTranslator{}.toFlat(m_dataRLE);
	m_dataFlat.expand();
	m_dataRLE.forget();
}
void vox::world::Chunk::contract()
{
	if (!m_dataRLE.empty())
		return;
	m_dataRLE = data::ChunkDataTranslator{}.toRLE(m_dataFlat);
	m_dataFlat.forget();
}
