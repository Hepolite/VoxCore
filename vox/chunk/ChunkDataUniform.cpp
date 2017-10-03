
#include "vox/chunk/ChunkDataUniform.h"

std::vector<vox::chunk::BlockData> vox::chunk::ChunkDataUniform::expand() const
{
	std::vector<BlockData> data;
	data.resize(VOLUME, m_block);
	return data;
}
void vox::chunk::ChunkDataUniform::fill(vox::chunk::BlockVolume& volume) const
{
	glm::ivec3 pos;
	for (pos.x = 0; pos.x < SIZE; ++pos.x)
	for (pos.y = 0; pos.y < SIZE; ++pos.y)
	for (pos.z = 0; pos.z < SIZE; ++pos.z)
		volume.setBlock(m_block.m_id, pos);
}
