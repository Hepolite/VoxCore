
#include "vox/chunk/ChunkDataRLE.h"

vox::chunk::ChunkDataRLE::ChunkDataRLE(const std::vector<BlockData>& data)
{
	BlockData currentData = data.front();
	unsigned int currentIndex = 0;
	for (const auto& d : data)
	{
		if (currentData != d)
		{
			m_data.emplace_back(currentIndex - 1, currentData);
			currentData = d;
		}
		++currentIndex;
	}
	m_data.emplace_back(VOLUME - 1, currentData);
}

vox::chunk::BlockData vox::chunk::ChunkDataRLE::getBlock(const glm::ivec3& pos) const
{
	int lower = 0;
	int upper = m_data.size() - 1;
	int center = upper / 2;

	const auto index = getIndex(pos);
	while (lower < upper)
	{
		const auto endIndex = m_data[center].m_endIndex;
		if (endIndex < index)
			lower = center + 1;
		else if (endIndex > index)
			upper = center;
		else
			return m_data[center].m_data;
		center = (lower + upper) / 2;
	}
	return m_data[center].m_data;
}
void vox::chunk::ChunkDataRLE::setBlock(const BlockData& block, const glm::ivec3& pos)
{
	throw std::exception("Not implemented");
}

std::vector<vox::chunk::BlockData> vox::chunk::ChunkDataRLE::expand() const
{
	std::vector<BlockData> blockData{};

	unsigned int index = 0;
	for (unsigned int i = 0; i < VOLUME; ++i)
	{
		const auto& rleData = m_data[index];
		blockData.push_back(rleData.m_data);
		if (i >= rleData.m_endIndex)
			++index;
	}
	return blockData;
}
void vox::chunk::ChunkDataRLE::fill(BlockVolume& volume) const
{
	unsigned int index = 0;
	for (unsigned int i = 0; i < VOLUME; ++i)
	{
		const auto& rleData = m_data[index];
		volume.setBlock(rleData.m_data.m_id, glm::ivec3{ i / (SIZE * SIZE), (i / SIZE) % SIZE, i % SIZE });
		if (i >= rleData.m_endIndex)
			++index;
	}
}
