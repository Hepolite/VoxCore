
#pragma once

#include "vox/world/Chunk.h"

#include <glm/gtx/hash.hpp>
#include <glm/vec3.hpp>

#include <unordered_set>

namespace vox
{
	namespace util
	{
		class LightPropagator
		{
		public:
			LightPropagator(world::Chunk* chunk) : m_chunk(chunk) {}

			void initialize();
			void work();

			inline auto& getInpactedChunks() const { return m_impactedChunks; }
			inline auto min() const { return m_min; }
			inline auto max() const { return m_max; }

		private:
			void updateBoundingBox(const glm::uvec3& pos);

			void propagate();
			void propagateThisChunk();
			void propagateOtherChunks();
			void propagateAlong(glm::uvec4 light, const glm::ivec3& pos, const world::Side& up);
			bool propagateTo(world::Chunk* chunk, const glm::uvec4& light, const glm::ivec3& target);
			bool propagateTo(data::ChunkDataFlat& data, glm::uvec4 light, const glm::ivec3& target);

			void removal();
			void removalThisChunk();
			void removalOtherChunks();
			void removalAlong(glm::uvec4 light, const glm::ivec3& pos, const world::Side& up);
			bool removalTo(world::Chunk* chunk, const glm::uvec4& light, const glm::ivec3& target);
			bool removalTo(data::ChunkDataFlat& data, const glm::uvec4& light, const glm::ivec3& target);

			std::unordered_set<world::Chunk*> m_chunksToPropagate;
			std::unordered_set<world::Chunk*> m_chunksToRemoval;
			std::unordered_set<world::Chunk*> m_impactedChunks;
			world::Chunk* m_chunk = nullptr;

			glm::uvec3 m_min{ std::numeric_limits<unsigned int>::max() };
			glm::uvec3 m_max{ std::numeric_limits<unsigned int>::min() };
		};
	}
}