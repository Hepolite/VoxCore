
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
			inline auto begin() const { return m_nodes.begin(); }
			inline auto end() { return m_nodes.end(); }
			inline auto end() const { return m_nodes.end(); }
			inline auto iter() const { return ChunkQueryIterator{ begin(), end() }; }

			bool add(BlockQuery&& query, const glm::ivec3& cpos);
			void add(const BlockData& data, const glm::ivec3& pos);
			bool has(const glm::ivec3& pos) const;
			BlockData get(const glm::ivec3& pos) const;

		private:
			QueryMap m_nodes;
			unsigned int m_memusage = 0;
		};
	}
}