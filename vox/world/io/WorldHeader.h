
#pragma once

#include <glm/gtx/hash.hpp>
#include <glm/vec3.hpp>

#include <unordered_set>

namespace vox
{
	namespace io
	{
		class WorldHeader
		{
			using ChunkSet = std::unordered_set<glm::ivec3>;

		public:
			WorldHeader();
			~WorldHeader();

			void saveToDisk();
			void loadFromDisk();

			void add(const glm::ivec3& chunk);
			void remove(const glm::ivec3& chunk);

			const ChunkSet& getChunks() const;

		private:
			ChunkSet m_chunks;
		};
	}
}