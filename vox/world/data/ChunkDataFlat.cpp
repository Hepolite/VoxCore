
#include "vox/world/data/ChunkDataFlat.h"

#include "vox/world/ChunkSize.h"

#include "hen/util/MathLib.h"

void vox::data::ChunkDataFlat::forget()
{
	m_data.swap(std::vector<BlockData>{});
}
void vox::data::ChunkDataFlat::expand()
{
	if (m_data.empty())
		m_data.resize(chunk::VOLUME, BlockData{});
}

vox::data::BlockData vox::data::ChunkDataFlat::getBlock(const glm::ivec3& pos) const
{
	const auto index = getIndex(pos);
	return index < m_data.size() ? m_data[index] : 0;
}
vox::data::BlockRegion vox::data::ChunkDataFlat::getBlocks() const
{
	BlockRegion region;
	glm::ivec3 pos;
	for (pos.x = 0; pos.x < chunk::SIZE; ++pos.x)
	for (pos.y = 0; pos.y < chunk::SIZE; ++pos.y)
	for (pos.z = 0; pos.z < chunk::SIZE; ++pos.z)
		region.setBlock(getBlock(pos), pos);
	return region;
}
vox::data::BlockRegion vox::data::ChunkDataFlat::getBlocks(const glm::ivec3& start, const glm::ivec3& end) const
{
	throw std::exception("Not implemented");
}
void vox::data::ChunkDataFlat::getBlocks(BlockRegion& region, const glm::ivec3& start, const glm::ivec3& end, const glm::ivec3& offset) const
{
	glm::ivec3 pos;
	for (pos.x = start.x; pos.x < end.x; ++pos.x)
	for (pos.y = start.y; pos.y < end.y; ++pos.y)
	for (pos.z = start.z; pos.z < end.z; ++pos.z)
		region.setBlock(getBlock(pos), pos + offset);
}

void vox::data::ChunkDataFlat::setBlock(const BlockData& block, const glm::ivec3& pos)
{
	const auto index = getIndex(pos);
	if (index < m_data.size())
		m_data[index] = block;
}
void vox::data::ChunkDataFlat::setBlocks(const BlockData& data)
{
	forget();
	m_data.resize(chunk::VOLUME, data);
}
void vox::data::ChunkDataFlat::setBlocks(const BlockRegion& data)
{
	expand();

	const auto max = hen::math::min(data.getOffset() + data.getSize(), glm::ivec3{ chunk::SIZE });
	const auto min = hen::math::max(data.getOffset(), glm::ivec3{});

	glm::ivec3 pos;
	for (pos.x = min.x; pos.x < max.x; ++pos.x)
	for (pos.y = min.y; pos.y < max.y; ++pos.y)
	for (pos.z = min.z; pos.z < max.z; ++pos.z)
		setBlock(data.getBlock(pos), pos);
}

unsigned int vox::data::ChunkDataFlat::getIndex(const glm::ivec3 & pos) const
{
	return (pos.x * chunk::SIZE + pos.y) * chunk::SIZE + pos.z;
}
