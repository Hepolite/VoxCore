
#pragma once

#include "vox/world/Chunk.h"

#include <glm/vec3.hpp>

#include <memory>
#include <unordered_map>

namespace vox
{
	namespace world
	{
		class ChunkColumn
		{
		public:
			ChunkColumn() = default;
			ChunkColumn(const ChunkColumn&) = delete;
			ChunkColumn(ChunkColumn&&) = default;
			~ChunkColumn() = default;

			ChunkColumn& operator=(const ChunkColumn&) = delete;
			ChunkColumn& operator=(ChunkColumn&&) = default;

			Chunk& createChunk(const glm::ivec3& cpos);
			Chunk* getChunkAt(int cz) const;
			Chunk* getChunkAbove(int cz) const;
			Chunk* getChunkBelow(int cz) const;
			Chunk* getTopChunk() const;
			Chunk* getBottomChunk() const;
			void deleteChunk(int cz);

			inline bool empty() const { return m_chunks.empty(); }
			inline auto begin() const { return m_chunks.begin(); }
			inline auto end() const { return m_chunks.end(); }

		private:
			std::unordered_map<int, std::unique_ptr<Chunk>> m_chunks;
			int m_top;
			int m_bottom;
		};
	}
}