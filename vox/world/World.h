
#pragma once

#include "vox/world/Chunk.h"
#include "vox/world/data/ChunkQuery.h"

#include <glm/gtx/hash.hpp>
#include <glm/vec3.hpp>

#include <mutex>
#include <string>
#include <unordered_map>

namespace vox
{
	namespace world
	{
		class World
		{
			using ChunkMap = std::unordered_map<glm::ivec3, Chunk>;

		public:
			World() = delete;
			World(const std::string& name) : m_name(name) {}

			inline const std::string& getName() const { return m_name; }

			Chunk& getOrCreateChunk(const glm::ivec3& cpos);
			void deleteChunk(const glm::ivec3& cpos);
			Chunk* getChunk(const glm::ivec3& cpos);
			const Chunk* getChunk(const glm::ivec3& cpos) const;
			std::vector<glm::ivec3> getChunks() const;

			data::BlockData getBlock(const glm::ivec3& pos) const;
			void acceptQuery(data::ChunkReadQuery& query) const;
			void acceptQuery(data::ChunkWriteQuery& query);

			void debugMemusage() const;

		private:
			const std::string m_name;

			ChunkMap m_chunks;
		};
	}
}