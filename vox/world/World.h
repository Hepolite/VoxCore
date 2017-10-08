
#pragma once

#include "vox/world/Chunk.h"
#include "vox/world/data/ChunkQuery.h"

#include <glm/gtx/hash.hpp>
#include <glm/vec3.hpp>

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

			inline std::string getName() const { return m_name; }

			data::BlockData getBlock(const glm::ivec3& pos) const;
			std::vector<glm::ivec3> getChunkCoordinates() const;

			void acceptQuery(data::ChunkReadQuery& query) const;
			void acceptQuery(data::ChunkWriteQuery& query);

			void injectChunkStorageData(const glm::ivec3& pos, data::ChunkDataRLE&& data);
			bool exportChunkStorageData(const glm::ivec3& pos, data::ChunkDataRLE& data) const;
			bool exportChunkRenderData(const glm::ivec3& pos, data::BlockRegion& data) const;

			void debugMemusage() const;

		private:
			Chunk& getOrCreateChunk(const glm::ivec3& cpos);
			void deleteChunk(const glm::ivec3& cpos);
			Chunk* getChunk(const glm::ivec3& cpos);
			const Chunk* getChunk(const glm::ivec3& cpos) const;

			const std::string m_name;

			ChunkMap m_chunks;
		};
	}
}