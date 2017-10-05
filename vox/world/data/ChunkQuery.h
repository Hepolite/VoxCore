
#pragma once

#include "vox/world/data/BlockQuery.h"

#include <glm/vec3.hpp>

#include <vector>

namespace vox
{
	namespace data
	{
		template<typename T> using ChunkQuery = std::pair<T, glm::ivec3>;
		template<typename T> using ChunkQueryList = std::vector<ChunkQuery<T>>;

		template<typename T>
		class ChunkBaseQuery
		{
		public:
			ChunkBaseQuery() = default;
			ChunkBaseQuery(const ChunkBaseQuery&) = delete;
			ChunkBaseQuery(ChunkBaseQuery&&) = default;

			ChunkBaseQuery& operator=(const ChunkBaseQuery&) = delete;
			ChunkBaseQuery& operator=(ChunkBaseQuery&&) = default;

			inline unsigned int size() const { return m_nodes.size(); }
			inline bool empty() const { return m_nodes.empty(); }

			inline auto begin() { return m_nodes.begin(); }
			inline auto end() { return m_nodes.end(); }

			inline void add(T&& query, const glm::ivec3& cpos)
			{
				if (!query.empty())
					m_nodes.emplace_back(std::move(query), cpos);
			}

		private:
			ChunkQueryList<T> m_nodes;
		};

		using ChunkReadQuery = ChunkBaseQuery<BlockReadQuery>;
		using ChunkWriteQuery = ChunkBaseQuery<BlockWriteQuery>;
	}
}