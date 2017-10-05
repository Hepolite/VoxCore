
#include "vox/world/data/ChunkData.h"

#include "vox/world/ChunkSize.h"

#include "hen/util/MathLib.h"

unsigned int vox::data::ChunkData::getIndex(const glm::uvec3& pos) const
{
	return (pos.x * chunk::SIZE + pos.y) * chunk::SIZE + pos.z;
}



vox::data::BlockData vox::data::ChunkDataFlat::getBlock(const glm::uvec3& pos) const
{
	return getBlock(getIndex(pos));
}
vox::data::BlockData vox::data::ChunkDataFlat::getBlock(unsigned int index) const
{
	return index < m_data.size() ? m_data[index] : 0;
}
void vox::data::ChunkDataFlat::swapBlock(BlockData& data, unsigned int index)
{
	if (index < m_data.size())
		std::swap(m_data[index], data);
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

void vox::data::ChunkDataFlat::fillRegion(BlockRegion& region, const glm::uvec3& dataOffset, const glm::ivec3& regionOffset, const glm::uvec3& size) const
{
	const auto min = hen::math::max(glm::uvec3{}, dataOffset);
	const auto max = hen::math::min(glm::uvec3{ static_cast<unsigned int>(chunk::SIZE) }, dataOffset + size);

	glm::uvec3 pos;
	for (pos.x = min.x; pos.x < max.x; ++pos.x)
	for (pos.y = min.y; pos.y < max.y; ++pos.y)
	for (pos.z = min.z; pos.z < max.z; ++pos.z)
		region.setBlock(getBlock(pos), glm::ivec3{ pos } + regionOffset);
}
void vox::data::ChunkDataFlat::acceptQuery(BlockReadQuery& query) const
{
	for (auto& node : query)
		node.first = getBlock(node.second) & query.bitmask();
}
void vox::data::ChunkDataFlat::acceptQuery(BlockWriteQuery& query)
{
	expand();
	for (auto& node : query)
		swapBlock(node.first, node.second);
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

bool vox::data::ChunkDataRLE::empty() const
{
	return m_data.empty() || (m_data.size() == 1 && m_data.front().first.getId() == 0);
}
void vox::data::ChunkDataRLE::forget()
{
	m_data.swap(NodeList{});
}

void vox::data::ChunkDataRLE::fillRegion(BlockRegion& region, const glm::uvec3& dataOffset, const glm::ivec3& regionOffset, const glm::uvec3& size) const
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
void vox::data::ChunkDataRLE::acceptQuery(BlockReadQuery& query) const
{
	for (auto& node : query)
		node.first = getBlock(node.second) & query.bitmask();
}
void vox::data::ChunkDataRLE::acceptQuery(BlockWriteQuery& query)
{
	throw std::exception("Not implemented");
}

