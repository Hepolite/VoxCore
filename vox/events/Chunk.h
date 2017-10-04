
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
			ChunkCreate(const world::World* world, const glm::ivec3& cpos) : m_world(world), m_cpos(cpos){}
			~ChunkCreate() = default;

			inline const world::World* getWorld() const { return m_world; }
			inline glm::ivec3 getChunkPos() const { return m_cpos; }

		private:
			const world::World* m_world;
			const glm::ivec3 m_cpos;
		};

		class ChunkDestroy
		{
		public:
			ChunkDestroy(const world::World* world, const glm::ivec3& cpos) : m_world(world), m_cpos(cpos) {}
			~ChunkDestroy() = default;

			inline const world::World* getWorld() const { return m_world; }
			inline glm::ivec3 getChunkPos() const { return m_cpos; }

		private:
			const world::World* m_world;
			const glm::ivec3 m_cpos;
		};

		class ChunkBlockChange
		{
		public:
			ChunkBlockChange(const world::Chunk* chunk, const world::World* world, const glm::ivec3& cpos, const glm::ivec3& start, const glm::ivec3& end)
				: m_chunk(chunk), m_world(world), m_cpos(cpos), m_start(start), m_end(end)
			{}
			~ChunkBlockChange() = default;

			inline const world::Chunk* getChunk() const { return m_chunk; }
			inline const world::World* getWorld() const { return m_world; }
			inline glm::ivec3 getChunkPos() const { return m_cpos; }
			inline glm::ivec3 getStart() const { return m_start; }
			inline glm::ivec3 getEnd() const { return m_end; }

		private:
			const world::Chunk* m_chunk;
			const world::World* m_world;
			const glm::ivec3 m_cpos;
			const glm::ivec3 m_start;
			const glm::ivec3 m_end;
		};
	}
}