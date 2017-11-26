
#include "vox/world/data/ChunkData.h"

#include "vox/VoxCore.h"
#include "vox/world/BlockRegistry.h"
#include "vox/world/data/Indexing.h"
#include "vox/world/Universe.h"

#include "hen/util/MathLib.h"


void vox::data::ChunkDataFlat::acceptRegionQuery(BlockRegion& region, const glm::uvec3& dataOffset, const glm::ivec3& regionOffset, const glm::uvec3& size) const
{
	const auto min = hen::math::max(glm::uvec3{}, dataOffset);
	const auto max = hen::math::min(glm::uvec3{ static_cast<unsigned int>(chunk::SIZE) }, dataOffset + size);

	glm::uvec3 pos;
	for (pos.x = min.x; pos.x < max.x; ++pos.x)
	for (pos.y = min.y; pos.y < max.y; ++pos.y)
	for (pos.z = min.z; pos.z < max.z; ++pos.z)
		region.setBlock(getBlock(pos), glm::ivec3{ pos } +regionOffset);
}
void vox::data::ChunkDataFlat::acceptReadQuery(BlockQuery& query) const
{
	for (auto& node : query)
		node.first = getBlock(node.second);
}
void vox::data::ChunkDataFlat::acceptWriteQuery(BlockQuery& query)
{
	expand();
	for (const auto node : query)
		setBlock(node.second, node.first);
}
void vox::data::ChunkDataFlat::setBlock(const glm::uvec3& pos, const BlockData& data)
{
	setBlock(getIndex(pos), data);
}
void vox::data::ChunkDataFlat::setBlock(unsigned int index, const BlockData& data)
{
	if (index < size())
	{
		if ((data.getData() & BlockData::BITMASK_LIGHT) != 0)
			m_lightPropagationNodes.emplace(data, index);
		else
			m_lightRemovalNodes.emplace(m_data[index], index);

		m_data[index] = data;
	}
}
vox::data::BlockData vox::data::ChunkDataFlat::getBlock(const glm::uvec3& pos) const
{
	return getBlock(getIndex(pos));
}
vox::data::BlockData vox::data::ChunkDataFlat::getBlock(unsigned int index) const
{
	return index < size() ? m_data[index] : data::BlockData{};
}

unsigned int vox::data::ChunkDataFlat::memusage() const
{
	return size() * sizeof(BlockData);
}
unsigned int vox::data::ChunkDataFlat::size() const
{
	return m_data.size();
}
bool vox::data::ChunkDataFlat::empty() const
{
	return m_data.empty();
}

void vox::data::ChunkDataFlat::expand()
{
	if (empty())
		m_data.resize(chunk::VOLUME, BlockData{});
}
void vox::data::ChunkDataFlat::forget()
{
	m_data.swap(BlockDataList{});
}



void vox::data::ChunkDataRLE::acceptRegionQuery(BlockRegion& region, const glm::uvec3& dataOffset, const glm::ivec3& regionOffset, const glm::uvec3& size) const
{
	const glm::uvec3 ZERO{};
	const glm::uvec3 SIZE{ static_cast<unsigned int>(chunk::SIZE) };

	if (dataOffset == ZERO && size == SIZE)
	{
		const auto min = regionOffset;
		const auto max = regionOffset + glm::ivec3{ size };

		unsigned int index = 0;
		unsigned int i = 0;
		glm::ivec3 pos;
		for (pos.x = min.x; pos.x < max.x; ++pos.x)
		for (pos.y = min.y; pos.y < max.y; ++pos.y)
		for (pos.z = min.z; pos.z < max.z; ++pos.z)
		{
			const auto& data = m_data.at(index);
			region.setBlock(data.first, pos);
			if (i++ >= data.second)
				++index;
		}
	}
	else
	{
		const auto min = hen::math::max(ZERO, dataOffset);
		const auto max = hen::math::min(SIZE, dataOffset + size);

		glm::uvec3 pos;
		for (pos.x = min.x; pos.x < max.x; ++pos.x)
		for (pos.y = min.y; pos.y < max.y; ++pos.y)
		for (pos.z = min.z; pos.z < max.z; ++pos.z)
			region.setBlock(getBlock(pos), glm::ivec3{ pos } + regionOffset);
	}
}
void vox::data::ChunkDataRLE::acceptReadQuery(BlockQuery& query) const
{
	for (auto& node : query)
		node.first = getBlock(node.second);
}
vox::data::BlockData vox::data::ChunkDataRLE::getBlock(const glm::uvec3& pos) const
{
	return getBlock(getIndex(pos));
}
vox::data::BlockData vox::data::ChunkDataRLE::getBlock(unsigned int index) const
{
	if (empty())
		return BlockData{};

	int lower = 0;
	int upper = m_data.size() - 1;
	int center = upper / 2;

	while (lower < upper)
	{
		const auto endIndex = m_data[center].second;
		if (endIndex < index)
			lower = center + 1;
		else if (endIndex > index)
			upper = center;
		else
			return m_data[center].first;
		center = (lower + upper) / 2;
	}
	return m_data[center].first;
}

unsigned int vox::data::ChunkDataRLE::memusage() const
{
	return m_data.size() * sizeof(Node);
}
unsigned int vox::data::ChunkDataRLE::size() const
{
	return m_data.size();
}
bool vox::data::ChunkDataRLE::empty() const
{
	return m_data.empty() || (m_data.size() == 1 && m_data.front().first.getData() == 0);
}

void vox::data::ChunkDataRLE::forget()
{
	m_data.swap(NodeList{});
}
