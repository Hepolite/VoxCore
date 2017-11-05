
#pragma once

#include "vox/world/data/BlockData.h"
#include "vox/world/data/BlockQuery.h"

#include <glm/vec3.hpp>
#include <glm/gtx/hash.hpp>

#include <unordered_map>
#include <vector>

namespace vox
{
	namespace data
	{
		class ChunkQueryIterator
		{
			using Query = std::pair<BlockData, unsigned short>;
			using QueryList = std::vector<Query>;
			using QueryMap = std::unordered_map<glm::ivec3, BlockQuery>;

		public:
			ChunkQueryIterator() = delete;
			ChunkQueryIterator(const QueryMap::const_iterator& begin, const QueryMap::const_iterator& end);

			bool isValid() const;
			void next();

			BlockData getData() const;
			glm::ivec3 getPos() const;

		private:
			QueryList::const_iterator m_currentBlock;
			QueryMap::const_iterator m_currentChunk;
			QueryMap::const_iterator m_end;
		};
	}
}