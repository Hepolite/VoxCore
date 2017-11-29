
#pragma once

#include <glm/vec3.hpp>

namespace vox
{
	namespace world
	{
		class Chunk;
		class World;
	}

	namespace events
	{
		class ChunkCreate
		{
		public:
			ChunkCreate(world::World* world, const glm::ivec3& cpos) : m_world(world), m_cpos(cpos) {}
			~ChunkCreate() = default;

			inline world::World* getWorld() const { return m_world; }
			inline glm::ivec3 getChunkPos() const { return m_cpos; }

		private:
			world::World* m_world;
			glm::ivec3 m_cpos;
		};

		class ChunkDestroy
		{
		public:
			ChunkDestroy(world::World* world, const glm::ivec3& cpos) : m_world(world), m_cpos(cpos) {}
			~ChunkDestroy() = default;

			inline world::World* getWorld() const { return m_world; }
			inline glm::ivec3 getChunkPos() const { return m_cpos; }

		private:
			world::World* m_world;
			glm::ivec3 m_cpos;
		};

		class ChunkChange
		{
		public:
			ChunkChange(world::World* world, world::Chunk* chunk, const glm::ivec3& cpos, const glm::ivec3& start, const glm::ivec3& end)
				: m_world(world), m_chunk(chunk), m_cpos(cpos), m_start(start), m_end(end)
			{}
			~ChunkChange() = default;

			inline world::World* getWorld() const { return m_world; }
			inline world::Chunk* getChunk() const { return m_chunk; }
			inline glm::ivec3 getChunkPos() const { return m_cpos; }
			inline glm::ivec3 getStart() const { return m_start; }
			inline glm::ivec3 getEnd() const { return m_end; }

		private:
			world::World* m_world;
			world::Chunk* m_chunk;
			glm::ivec3 m_cpos;
			glm::ivec3 m_start;
			glm::ivec3 m_end;
		};
	}
}