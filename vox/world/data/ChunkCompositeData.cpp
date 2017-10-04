
#include "vox/world/data/ChunkCompositeData.h"

#include "vox/world/data/ChunkDataTranslator.h"

void vox::data::ChunkCompositeData::optimizeDataFormat()
{
	if (m_format == DataFormat::NONE)
		return;
	setDataFormat(DataFormat::UNIFORM);
	if (getDataFormat() != DataFormat::UNIFORM)
		setDataFormat(DataFormat::RLE);
	else
	{
		if (m_dataUniform.getBlock() == data::BlockData{})
			setDataFormat(DataFormat::NONE);
	}
}
void vox::data::ChunkCompositeData::setDataFormat(DataFormat format)
{
	if (m_format == format)
		return;

	bool forgetFlat = true;
	bool forgetRLE = true;
	bool forgetUniform = true;

	const data::ChunkDataTranslator translator;
	switch (format)
	{
	case DataFormat::FLAT:
		if (m_format == DataFormat::RLE)
			m_dataFlat = translator.toFlat(m_dataRLE);
		else if (m_format == DataFormat::UNIFORM)
			m_dataFlat = translator.toFlat(m_dataUniform);
		else
			m_dataFlat.expand();
		forgetFlat = false;
		break;

	case DataFormat::RLE:
		if (m_format == DataFormat::FLAT)
			m_dataRLE = translator.toRLE(m_dataFlat);
		else if (m_format == DataFormat::UNIFORM)
			m_dataRLE = translator.toRLE(m_dataUniform);
		forgetRLE = false;
		break;

	case DataFormat::UNIFORM:
	{
		std::optional<data::ChunkDataUniform> data{};
		if (m_format == DataFormat::FLAT)
			data = translator.toUniform(m_dataFlat);
		else if (m_format == DataFormat::RLE)
			data = translator.toUniform(m_dataRLE);
		else
			data = data::ChunkDataUniform{};
		if (data) // The data cannot always be compressed to the uniform state
		{
			m_dataUniform = *data;
			forgetUniform = false;
		}
		else // If the uniform state is unobtainable, discard change
			return;
		break;
	}

	default:
		break;
	}

	if (forgetFlat)		m_dataFlat.forget();
	if (forgetRLE)		m_dataRLE.forget();
	if (forgetUniform)	m_dataUniform.forget();
	m_format = format;
}

vox::data::BlockQuery vox::data::ChunkCompositeData::requestBlock(const glm::ivec3& pos) const
{
	switch (m_format)
	{
	case DataFormat::FLAT:
		return BlockQuery{ m_dataFlat.getBlock(pos), pos };
		break;
	case DataFormat::RLE:
		return BlockQuery{ m_dataRLE.getBlock(pos), pos };
		break;
	case DataFormat::UNIFORM:
		return BlockQuery{ m_dataUniform.getBlock(), pos };
		break;
	}
	return BlockQuery{};
}
vox::data::BlockQueryList vox::data::ChunkCompositeData::requestBlocks(const std::vector<glm::ivec3>& locations) const
{
	BlockQueryList::QueryList list;
	switch (m_format)
	{
	case DataFormat::FLAT:
		for (const auto& location : locations)
			list.emplace_back(m_dataFlat.getBlock(location), location);
		break;
	case DataFormat::RLE:
		for (const auto& location : locations)
			list.emplace_back(m_dataRLE.getBlock(location), location);
		break;
	case DataFormat::UNIFORM:
		const auto block = m_dataUniform.getBlock();
		for (const auto& location : locations)
			list.emplace_back(block, location);
		break;
	}
	return BlockQueryList{ std::move(list) };
}
vox::data::BlockQueryRectangle vox::data::ChunkCompositeData::requestBlocks(const glm::ivec3& start, const glm::ivec3& end) const
{
	switch (m_format)
	{
	case DataFormat::FLAT:
		return m_dataFlat.getBlocks(start, end);
	case DataFormat::RLE:
		return m_dataRLE.getBlocks(start, end);
	case DataFormat::UNIFORM:
		return m_dataUniform.getBlocks(start, end);
	}
	return BlockQueryRectangle{ BlockRegion{ start, end } };
}
void vox::data::ChunkCompositeData::reguestBlocks(BlockRegion& region, const glm::ivec3& start, const glm::ivec3& end, const glm::ivec3& offset) const
{
	switch (m_format)
	{
	case DataFormat::FLAT:
		m_dataFlat.getBlocks(region, start, end, offset);
		break;
	case DataFormat::RLE:
		m_dataRLE.getBlocks(region, start, end, offset);
		break;
	case DataFormat::UNIFORM:
		region.setBlocks(m_dataUniform.getBlock(), start + offset, end + offset);
		break;
	}
}

void vox::data::ChunkCompositeData::assignChange(const BlockQuery& query)
{
	switch (m_format)
	{
	case DataFormat::FLAT:
		m_dataFlat.setBlock(query.m_data, query.m_pos);
		break;
	case DataFormat::RLE:
		m_dataRLE.setBlock(query.m_data, query.m_pos);
		break;
	case DataFormat::UNIFORM:
		m_dataUniform.setBlock(query.m_data);
		break;
	}
}
void vox::data::ChunkCompositeData::assignChange(const BlockQueryList& query)
{
	switch (m_format)
	{
	case DataFormat::FLAT:
		for (const auto& q : query.m_data)
			m_dataFlat.setBlock(q.m_data, q.m_pos);
		break;
	case DataFormat::RLE:
		for (const auto& q : query.m_data)
			m_dataRLE.setBlock(q.m_data, q.m_pos);
		break;
	case DataFormat::UNIFORM:
		if (!query.m_data.empty())
			m_dataUniform.setBlock(query.m_data.back().m_data);
		break;
	}
}
void vox::data::ChunkCompositeData::assignChange(const BlockQueryRectangle& query)
{
	switch (m_format)
	{
	case DataFormat::FLAT:
		m_dataFlat.setBlocks(query.m_data);
		break;
	case DataFormat::RLE:
		m_dataRLE.setBlocks(query.m_data);
		break;
	case DataFormat::UNIFORM:
		m_dataUniform.setBlocks(query.m_data);
		break;
	}
}
