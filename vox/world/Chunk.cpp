
#include "vox/world/Chunk.h"

#include "vox/world/ChunkSize.h"

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

vox::data::BlockQuery vox::world::Chunk::getBlock(const glm::ivec3& pos) const
{
	return m_data.requestBlock(pos);
}
vox::data::BlockQueryList vox::world::Chunk::getBlocks(const std::vector<glm::ivec3>& locations) const
{
	return m_data.requestBlocks(locations);
}
vox::data::BlockQueryRectangle vox::world::Chunk::getBlocks(const glm::ivec3& start, const glm::ivec3& end) const
{
	return m_data.requestBlocks(start, end);
}

void vox::world::Chunk::setBlock(const data::BlockQuery& query)
{
	m_data.setDataFormat(data::ChunkCompositeData::DataFormat::FLAT);
	m_data.assignChange(query);
	m_data.optimizeDataFormat();
}
void vox::world::Chunk::setBlocks(const data::BlockQueryList& query)
{
	m_data.setDataFormat(data::ChunkCompositeData::DataFormat::FLAT);
	m_data.assignChange(query);
	m_data.optimizeDataFormat();
}
void vox::world::Chunk::setBlocks(const data::BlockQueryRectangle& query)
{
	m_data.setDataFormat(data::ChunkCompositeData::DataFormat::FLAT);
	m_data.assignChange(query);
	m_data.optimizeDataFormat();
}

vox::data::BlockRegion vox::world::Chunk::getMeshingData() const
{
	const glm::ivec3 min{ 0 };
	const glm::ivec3 max{ chunk::SIZE };

	data::BlockRegion data{ glm::ivec3{ -1 }, glm::ivec3{ chunk::SIZE + 2 } };

	m_data.reguestBlocks(data, min, max, glm::ivec3{});
	if (const auto neightbor = getNeighbor(Side::NORTH))
		neightbor->m_data.reguestBlocks(data, min, glm::ivec3{ min.x + 1, max.y, max.z }, glm::ivec3{ chunk::SIZE, 0, 0 });
	if (const auto neightbor = getNeighbor(Side::SOUTH))
		neightbor->m_data.reguestBlocks(data, glm::ivec3{ max.x - 1, min.y, min.z }, max, glm::ivec3{ -chunk::SIZE, 0, 0 });
	if (const auto neightbor = getNeighbor(Side::WEST))
		neightbor->m_data.reguestBlocks(data, min, glm::ivec3{ max.x, min.y + 1, max.z }, glm::ivec3{ 0, chunk::SIZE, 0 });
	if (const auto neightbor = getNeighbor(Side::EAST))
		neightbor->m_data.reguestBlocks(data, glm::ivec3{ min.x, max.y - 1, min.z }, max, glm::ivec3{ 0, -chunk::SIZE, 0 });
	if (const auto neightbor = getNeighbor(Side::TOP))
		neightbor->m_data.reguestBlocks(data, min, glm::ivec3{ max.x, max.y, min.z + 1 }, glm::ivec3{ 0, 0, chunk::SIZE });
	if (const auto neightbor = getNeighbor(Side::BOTTOM))
		neightbor->m_data.reguestBlocks(data, glm::ivec3{ min.x, min.y, max.z - 1 }, max, glm::ivec3{ 0, 0, -chunk::SIZE });

	return data;
}
vox::data::ChunkDataRLE vox::world::Chunk::getCompressedData() const
{
	return data::ChunkDataRLE{};
}

bool vox::world::Chunk::isEmpty() const
{
	return m_data.getDataFormat() == data::ChunkCompositeData::DataFormat::NONE;
}

