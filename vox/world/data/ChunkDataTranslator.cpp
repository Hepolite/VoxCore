
#include "vox/world/data/ChunkDataTranslator.h"

#include "vox/world/ChunkSize.h"

vox::data::ChunkDataFlat vox::data::ChunkDataTranslator::toFlat(const ChunkDataRLE& rleData) const
{
	if (rleData.empty())
		return {};

	ChunkDataFlat flatData;
	flatData.expand();
	unsigned int index = 0;
	for (unsigned int i = 0; i < chunk::VOLUME; ++i)
	{
		const auto& node = rleData.m_data.at(index);
		flatData.m_data[i] = node.first;
		if (i >= node.second)
			++index;
	}
	return flatData;
}

vox::data::ChunkDataRLE vox::data::ChunkDataTranslator::toRLE(const ChunkDataFlat& flatData) const
{
	if (flatData.empty())
		return {};

	ChunkDataRLE rleData;
	BlockData currentData = flatData.m_data.front();
	unsigned int currentIndex = 0;
	for (const auto& data : flatData.m_data)
	{
		if (currentData != data)
		{
			rleData.m_data.emplace_back(currentData, currentIndex - 1);
			currentData = data;
		}
		++currentIndex;
	}
	rleData.m_data.emplace_back(currentData, chunk::VOLUME - 1);
	return rleData;
}
