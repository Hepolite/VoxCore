
#include "vox/world/data/BlockRegion.h"

vox::data::BlockRegion::BlockRegion(const glm::ivec3& offset, const glm::ivec3& size)
	: BlockRegion(offset, size, BlockData{})
{}
vox::data::BlockRegion::BlockRegion(const glm::ivec3& offset, const glm::ivec3& size, const BlockData& data)
	: m_offset(offset), m_size(size)
{
	m_data.resize(m_size.x * m_size.y * m_size.z, data);
}

unsigned int vox::data::BlockRegion::getIndex(const glm::ivec3& pos) const
{
	return ((pos.x - m_offset.x) * m_size.y + (pos.y - m_offset.y)) * m_size.z + (pos.z - m_offset.z);
}

vox::data::BlockData vox::data::BlockRegion::getBlock(const glm::ivec3& pos) const
{
	const auto index = getIndex(pos);
	return index < m_data.size() ? m_data[index] : BlockData{};
}
void vox::data::BlockRegion::setBlock(const BlockData& block, const glm::ivec3& pos)
{
	const auto index = getIndex(pos);
	if (index < m_data.size())
		m_data[index] = block;
}
void vox::data::BlockRegion::setBlocks(const BlockData& block, const glm::ivec3& start, const glm::ivec3& end)
{
	glm::ivec3 pos;
	for (pos.x = start.x; pos.x < end.x; ++pos.x)
	for (pos.y = start.y; pos.y < end.y; ++pos.y)
	for (pos.z = start.z; pos.z < end.z; ++pos.z)
		setBlock(block, pos);
}
