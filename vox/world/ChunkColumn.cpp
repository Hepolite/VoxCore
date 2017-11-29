
#include "vox/world/ChunkColumn.h"

#include "hen/util/MathLib.h"

vox::world::Chunk& vox::world::ChunkColumn::createChunk(const glm::ivec3& cpos)
{
	if (m_chunks.empty())
	{
		m_top = cpos.z;
		m_bottom = cpos.z;
	}
	else
	{
		m_top = hen::math::max(m_top, cpos.z);
		m_bottom = hen::math::min(m_bottom, cpos.z);
	}

	return *m_chunks.emplace(cpos.z, std::make_unique<Chunk>(cpos)).first->second;
}

vox::world::Chunk* vox::world::ChunkColumn::getChunkAt(int cz) const
{
	const auto it = m_chunks.find(cz);
	return it == m_chunks.end() ? nullptr : it->second.get();
}
vox::world::Chunk* vox::world::ChunkColumn::getChunkAbove(int cz) const
{
	int current = std::numeric_limits<int>::max();
	for (auto& node : m_chunks)
	{
		if (node.first > cz)
			current = hen::math::min(current, node.first);
	}
	return getChunkAt(current);
}
vox::world::Chunk* vox::world::ChunkColumn::getChunkBelow(int cz) const
{
	int current = std::numeric_limits<int>::min();
	for (auto& node : m_chunks)
	{
		if (node.first < cz)
			current = hen::math::max(current, node.first);
	}
	return getChunkAt(current);
}
vox::world::Chunk * vox::world::ChunkColumn::getTopChunk() const
{
	return getChunkAt(m_top);
}
vox::world::Chunk * vox::world::ChunkColumn::getBottomChunk() const
{
	return getChunkAt(m_bottom);
}

void vox::world::ChunkColumn::deleteChunk(int cz)
{
	m_chunks.erase(cz);

	if (!m_chunks.empty())
	{
		if (m_top == cz)
		{
			m_top = std::numeric_limits<int>::min();
			for (auto& node : m_chunks)
				m_top = hen::math::max(m_top, node.first);
		}
		if (m_bottom == cz)
		{
			m_bottom = std::numeric_limits<int>::max();
			for (auto& node : m_chunks)
				m_bottom = hen::math::min(m_bottom, node.first);
		}
	}
}
