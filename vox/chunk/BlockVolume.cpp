
#include "vox/chunk/BlockVolume.h"

vox::chunk::BlockVolume::BlockVolume(unsigned int size, const glm::ivec3& offset) :
	m_size(size),
	m_volume(size * size * size),
	m_offset(offset)
{
	m_data.resize(m_volume, BlockData{});
}
vox::chunk::BlockVolume::~BlockVolume(){}

vox::chunk::BlockData vox::chunk::BlockVolume::getBlock(const glm::ivec3& pos) const
{
	const auto& index = getIndex(pos);
	return index < m_volume ? m_data[index] : 0;
}
void vox::chunk::BlockVolume::setBlock(const BlockData& block, const glm::ivec3& pos)
{
	const auto& index = getIndex(pos);
	if (index < m_volume)
		m_data[index] = block;
}

unsigned int vox::chunk::BlockVolume::getIndex(const glm::ivec3& pos) const
{
	glm::uvec3 realPos{ pos - m_offset };
	return (realPos.x * m_size + realPos.y) * m_size + realPos.z;
}
