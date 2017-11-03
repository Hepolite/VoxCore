
#pragma once

#include "vox/world/data/BlockQuery.h"
#include "vox/world/data/ChunkQueryIterator.h"

#include <glm/vec3.hpp>
#include <glm/gtx/hash.hpp>

#include <unordered_map>

namespace vox
{
	namespace data
	{
		const unsigned int MAX_BYTES_PER_QUERY = 512 * 1024 * 1024;

		class ChunkQuery
		{
			using QueryMap = std::unordered_map<glm::ivec3, BlockQuery>;

		public:
			ChunkQuery() = default;
			ChunkQuery(const ChunkQuery&) = delete;
			ChunkQuery(ChunkQuery&&) = default;

			ChunkQuery& operator=(const ChunkQuery&) = delete;
			ChunkQuery& operator=(ChunkQuery&&) = default;

			inline unsigned int memusage() const { return m_memusage; }
			inline unsigned int size() const { return m_nodes.size(); }
			inline bool empty() const { return m_nodes.empty(); }

			inline auto begin() { return m_nodes.begin(); }
			inline auto end() { return m_nodes.end(); }
			inline auto iter() { return ChunkQueryIterator{ begin(), end() }; }

			inline bool add(BlockQuery&& query, const glm::ivec3& cpos)
			{
				if (m_memusage + query.memusage() > MAX_BYTES_PER_QUERY)
					return false;
				m_memusage += query.memusage();
				if (!query.empty())
					m_nodes[cpos] = std::move(query);
				return true;
			}

		private:
			QueryMap m_nodes;
			unsigned int m_memusage = 0;
		};
	}
}