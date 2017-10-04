
#include "vox/world/data/ChunkDataRLE.h"

#include "vox/world/ChunkSize.h"

#include "hen/util/MathLib.h"

void vox::data::ChunkDataRLE::forget()
{
	m_data.swap(std::vector<RLEData>{});
}

vox::data::BlockData vox::data::ChunkDataRLE::getBlock(const glm::ivec3& pos) const
{
	if (m_data.empty())
		return BlockData{};

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
vox::data::BlockRegion vox::data::ChunkDataRLE::getBlocks() const
{
	BlockRegion region;
	unsigned int index = 0;
	for (unsigned int i = 0; i < chunk::SIZE; ++i)
	{
		region.getRawData()[i] = m_data[index].m_data;
		if (i >= m_data[index].m_endIndex)
			++index;
	}
	return region;
}
vox::data::BlockRegion vox::data::ChunkDataRLE::getBlocks(const glm::ivec3& start, const glm::ivec3& end) const
{
	throw std::exception("Not implemented");
}
void vox::data::ChunkDataRLE::getBlocks(BlockRegion& region, const glm::ivec3& start, const glm::ivec3& end, const glm::ivec3& offset) const
{
	glm::ivec3 pos;
	for (pos.x = start.x; pos.x < end.x; ++pos.x)
	for (pos.y = start.y; pos.y < end.y; ++pos.y)
	for (pos.z = start.z; pos.z < end.z; ++pos.z)
		region.setBlock(getBlock(pos), pos + offset);
}

void vox::data::ChunkDataRLE::setBlock(const BlockData& block, const glm::ivec3& pos)
{
	throw std::exception("Not implemented");
}
void vox::data::ChunkDataRLE::setBlocks(const BlockData& data)
{
	forget();
	m_data.emplace_back(chunk::VOLUME - 1, data);
}
void vox::data::ChunkDataRLE::setBlocks(const BlockRegion& data)
{
	throw std::exception("Not implemented");
}
void vox::data::ChunkDataRLE::setBlocks(const std::vector<BlockData>& data)
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
	m_data.emplace_back(chunk::VOLUME - 1, currentData);
}

unsigned int vox::data::ChunkDataRLE::getIndex(const glm::ivec3 & pos) const
{
	return (pos.x * chunk::SIZE + pos.y) * chunk::SIZE + pos.z;
}
