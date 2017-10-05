
#include "vox/world/data/BlockQuery.h"

#include "vox/world/ChunkSize.h"

#include "hen/util/MathLib.h"

unsigned int vox::data::BlockBaseQuery::getIndex(const glm::uvec3& pos) const
{
	return (pos.x * chunk::SIZE + pos.y) * chunk::SIZE + pos.z;
}
void vox::data::BlockBaseQuery::limit(const glm::uvec3& lower, const glm::uvec3& upper)
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

void vox::data::BlockReadQuery::add(const glm::uvec3& pos)
{
	m_nodes.emplace_back(0, getIndex(pos));
}
void vox::data::BlockReadQuery::add(const glm::uvec3& start, const glm::uvec3& end)
{
	glm::uvec3 pos;
	for (pos.x = start.x; pos.x <= end.x; ++pos.x)
	for (pos.y = start.y; pos.y <= end.y; ++pos.y)
	for (pos.z = start.z; pos.z <= end.z; ++pos.z)
		m_nodes.emplace_back(0, getIndex(pos));
}

void vox::data::BlockWriteQuery::add(const BlockData& data, const glm::uvec3& pos)
{
	limit(pos, pos);
	m_nodes.emplace_back(data, getIndex(pos));
}
void vox::data::BlockWriteQuery::add(const BlockData& data, const glm::uvec3& start, const glm::uvec3& end)
{
	limit(hen::math::min(start, end), hen::math::max(start, end));

	glm::uvec3 pos;
	for (pos.x = start.x; pos.x <= end.x; ++pos.x)
	for (pos.y = start.y; pos.y <= end.y; ++pos.y)
	for (pos.z = start.z; pos.z <= end.z; ++pos.z)
		m_nodes.emplace_back(data, getIndex(pos));
}
