
#include "vox/world/data/BlockQuery.h"

#include "vox/world/data/Indexing.h"

#include "hen/util/MathLib.h"

void vox::data::BlockQuery::add(const BlockData& data, const glm::uvec3& pos)
{
	limit(pos, pos);
	set(data, pos);
}
void vox::data::BlockQuery::add(const BlockData& data, const glm::uvec3& start, const glm::uvec3& end)
{
	const auto min = hen::math::min(start, end);
	const auto max = hen::math::max(start, end);
	limit(min, max);

	glm::uvec3 pos;
	for (pos.x = min.x; pos.x <= max.x; ++pos.x)
	for (pos.y = min.y; pos.y <= max.y; ++pos.y)
	for (pos.z = min.z; pos.z <= max.z; ++pos.z)
		set(data, pos);
}
bool vox::data::BlockQuery::has(const glm::uvec3& pos) const
{
	auto loc = getLocation(pos);
	return loc != m_nodes.end() && loc->second == getIndex(pos);
}
void vox::data::BlockQuery::set(const BlockData& data, const glm::uvec3& pos)
{
	const auto index = getIndex(pos);
	if (empty() || m_nodes.back().second < index)
		m_nodes.emplace_back(data, index);
	else
	{
		auto loc = getLocation(pos, data);
		if (loc->second == index)
			loc->first = data;
		else
			m_nodes.insert(loc, std::make_pair(data, index));
	}
}
vox::data::BlockData vox::data::BlockQuery::get(const glm::uvec3& pos) const
{
	const auto it = getLocation(pos);
	return it == m_nodes.end() ? BlockData{} : it->first;
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

vox::data::QueryList::iterator vox::data::BlockQuery::getLocation(const glm::uvec3& pos, const BlockData& data)
{
	return std::upper_bound(m_nodes.begin(), m_nodes.end(), std::make_pair(data, getIndex(pos)),
		[](auto& lhs, auto& rhs) { return lhs.second <= rhs.second; }
	);
}
vox::data::QueryList::const_iterator vox::data::BlockQuery::getLocation(const glm::uvec3& pos) const
{
	return std::upper_bound(m_nodes.begin(), m_nodes.end(), std::make_pair(BlockData{}, getIndex(pos)),
		[](auto& lhs, auto& rhs) { return lhs.second <= rhs.second; }
	);
}
