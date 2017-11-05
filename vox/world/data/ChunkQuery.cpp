
#include "vox/world/data/ChunkQuery.h"

#include "vox/world/ChunkSize.h"

bool vox::data::ChunkQuery::add(BlockQuery&& query, const glm::ivec3& cpos)
{
	if (m_memusage + query.memusage() > MAX_BYTES_PER_QUERY)
		return false;
	m_memusage += query.memusage();
	if (!query.empty())
		m_nodes[cpos] = std::move(query);
	return true;
}

void vox::data::ChunkQuery::add(const BlockData& data, const glm::ivec3& pos)
{
	m_nodes[pos >> chunk::SIZE_LG].add(data, pos & chunk::SIZE_MINUS_ONE);
}
bool vox::data::ChunkQuery::has(const glm::ivec3& pos) const
{
	const auto node = m_nodes.find(pos >> chunk::SIZE_LG);
	return node == m_nodes.end() ? false : node->second.has(pos & chunk::SIZE_MINUS_ONE);
}
vox::data::BlockData vox::data::ChunkQuery::get(const glm::ivec3& pos) const
{
	const auto node = m_nodes.find(pos >> chunk::SIZE_LG);
	return node == m_nodes.end() ? BlockData{} : node->second.get(pos & chunk::SIZE_MINUS_ONE);
}
