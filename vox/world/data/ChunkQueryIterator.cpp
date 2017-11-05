
#include "vox/world/data/ChunkQueryIterator.h"

#include "vox/world/ChunkSize.h"

namespace
{
	inline glm::ivec3 getPosition(unsigned short index)
	{
		return glm::ivec3{
			index / (vox::chunk::SIZE * vox::chunk::SIZE),
			(index / vox::chunk::SIZE) % vox::chunk::SIZE,
			index % vox::chunk::SIZE
		};
	}
}

vox::data::ChunkQueryIterator::ChunkQueryIterator(const QueryMap::const_iterator& begin, const QueryMap::const_iterator& end)
	: m_currentChunk(begin), m_end(end)
{
	if (isValid())
		m_currentBlock = m_currentChunk->second.begin();
}

bool vox::data::ChunkQueryIterator::isValid() const
{
	return m_currentChunk != m_end;
}
void vox::data::ChunkQueryIterator::next()
{
	if (!isValid())
		throw std::exception("Attempted to step to next element after reaching end of iteration");
	if (++m_currentBlock == m_currentChunk->second.end())
	{
		if (++m_currentChunk != m_end)
			m_currentBlock = m_currentChunk->second.begin();
	}
}

vox::data::BlockData vox::data::ChunkQueryIterator::getData() const
{
	return m_currentBlock->first;
}
glm::ivec3 vox::data::ChunkQueryIterator::getPos() const
{
	return getPosition(m_currentBlock->second) + m_currentChunk->first * chunk::SIZE;
}

