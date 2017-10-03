
#include "vox/chunk/ChunkDataFlat.h"

vox::chunk::ChunkDataFlat::ChunkDataFlat()
{
	m_data.resize(VOLUME, 0);
}

vox::chunk::BlockData vox::chunk::ChunkDataFlat::getBlock(const glm::ivec3& pos) const
{
	const auto index = getIndex(pos);
	return index < m_data.size() ? m_data[index] : 0;
}
void vox::chunk::ChunkDataFlat::setBlock(const BlockData& block, const glm::ivec3& pos)
{
	const auto index = getIndex(pos);
	if (index < m_data.size())
		m_data[index] = block;
}

void vox::chunk::ChunkDataFlat::fill(BlockVolume& volume) const
{
	glm::ivec3 pos;
	for (pos.x = 0; pos.x < SIZE; ++pos.x)
	for (pos.y = 0; pos.y < SIZE; ++pos.y)
	for (pos.z = 0; pos.z < SIZE; ++pos.z)
		volume.setBlock(getBlock(pos).m_id, pos);
}
