
#include "vox/world/data/BlockQuery.h"

#include "vox/world/ChunkSize.h"

#include "hen/util/MathLib.h"

unsigned short vox::data::BlockQuery::getIndex(const glm::uvec3& pos) const
{
	return (pos.x * chunk::SIZE + pos.y) * chunk::SIZE + pos.z;
}

void vox::data::BlockQuery::add(const BlockData& data, const glm::uvec3& pos)
{
	limit(pos, pos);
	set(data, pos);
}
void vox::data::BlockQuery::add(const BlockData& data, const glm::uvec3& start, const glm::uvec3& end)
{
	const auto min = hen::math::min(start, end);
	const auto max = hen::math::max(start, end);

	glm::uvec3 pos;
	for (pos.x = min.x; pos.x <= max.x; ++pos.x)
	for (pos.y = min.y; pos.y <= max.y; ++pos.y)
	for (pos.z = min.z; pos.z <= max.z; ++pos.z)
		set(data, pos);
}
void vox::data::BlockQuery::set(const BlockData& data, const glm::uvec3& pos)
{
	const auto index = getIndex(pos);
	if (empty() || m_nodes.back().second < index)
		m_nodes.emplace_back(data, index);
	else
	{
		const auto node = std::make_pair(data, index);
		auto it = std::upper_bound(m_nodes.begin(), m_nodes.end(), node,
			[](auto& lhs, auto& rhs) { return lhs.second <= rhs.second; }
		);
		m_nodes.insert(it, node);
	}
}
bool vox::data::BlockQuery::get(vox::data::BlockData& data, const glm::uvec3& pos) const
{
	const auto node = std::make_pair(BlockData{}, getIndex(pos));
	auto it = std::upper_bound(m_nodes.begin(), m_nodes.end(), node,
		[](auto& lhs, auto& rhs) { return lhs.second <= rhs.second; }
	);
	if (it == m_nodes.end())
		return false;
	data = it->first;
	return true;
}

void vox::data::BlockQuery::limit(const glm::uvec3& lower, const glm::uvec3& upper)
{
	if (empty())
	{
		m_min = lower;
		m_max = upper;
	}
	else
	{
		m_min = hen::math::max(m_min, lower);
		m_max = hen::math::min(m_max, upper);
	}
}
