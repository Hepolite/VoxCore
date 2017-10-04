
#include "vox/world/data/ChunkDataTranslator.h"

#include "vox/world/ChunkSize.h"

vox::data::ChunkDataFlat vox::data::ChunkDataTranslator::toFlat(const ChunkDataRLE& data) const
{
	ChunkDataFlat newData;
	newData.expand();
	unsigned int index = 0;
	for (unsigned int i = 0; i < chunk::VOLUME; ++i)
	{
		const auto rleData = data.getRawData()[index];
		newData.getRawData()[i] = rleData.m_data;
		if (i >= rleData.m_endIndex)
			++index;
	}
	return newData;
}
vox::data::ChunkDataFlat vox::data::ChunkDataTranslator::toFlat(const ChunkDataUniform& data) const
{
	return ChunkDataFlat{ data.getBlock() };
}

vox::data::ChunkDataRLE vox::data::ChunkDataTranslator::toRLE(const ChunkDataFlat& data) const
{
	return ChunkDataRLE{ data.getRawData() };
}
vox::data::ChunkDataRLE vox::data::ChunkDataTranslator::toRLE(const ChunkDataUniform& data) const
{
	return ChunkDataRLE{ data.getBlock() };
}

std::optional<vox::data::ChunkDataUniform> vox::data::ChunkDataTranslator::toUniform(const ChunkDataFlat& data) const
{
	const auto& rawData = data.getRawData();
	if (rawData.empty())
		return ChunkDataUniform{};
	
	const auto& begin = std::begin(rawData);
	if (std::equal(begin + 1, std::end(rawData), begin))
		return ChunkDataUniform{ *begin };
	return {};
}
std::optional<vox::data::ChunkDataUniform> vox::data::ChunkDataTranslator::toUniform(const ChunkDataRLE& data) const
{
	const auto& rawData = data.getRawData();
	if (rawData.size() == 1)
		return ChunkDataUniform{ rawData.front().m_data };
	return {};
}
