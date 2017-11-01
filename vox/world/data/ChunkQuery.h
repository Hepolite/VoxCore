
#pragma once

#include "vox/world/data/BlockQuery.h"

#include <glm/vec3.hpp>

#include <vector>

namespace vox
{
	namespace data
	{
		const unsigned int MAX_BYTES_PER_QUERY = 512 * 1024 * 1024;

		class ChunkQuery
		{
			using Query = std::pair<BlockQuery, glm::ivec3>;
			using QueryList = std::vector<Query>;

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

			inline bool add(BlockQuery&& query, const glm::ivec3& cpos)
			{
				if (m_memusage + query.memusage() > MAX_BYTES_PER_QUERY)
					return false;
				m_memusage += query.memusage();
				if (!query.empty())
					m_nodes.emplace_back(std::move(query), cpos);
				return true;
			}

		private:
			QueryList m_nodes;
			unsigned int m_memusage = 0;
		};
	}
}