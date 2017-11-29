
#pragma once

#include "vox/world/ChunkColumn.h"
#include "vox/world/data/ChunkQuery.h"

#include <glm/gtx/hash.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <string>
#include <unordered_map>

namespace vox
{
	namespace world
	{
		class World
		{
		public:
			World() = delete;
			World(const std::string& name) : m_name(name) {}
			World(const World&) = delete;
			World(World&&) = delete;
			~World() = default;

			World& operator=(const World&) = delete;
			World& operator=(World&&) = delete;


			inline std::string getName() const { return m_name; }
			std::vector<glm::ivec3> getChunkCoordinates() const;

			data::BlockData getBlock(const glm::ivec3& pos) const;
			void acceptReadQuery(data::ChunkQuery& query) const;
			void acceptWriteQuery(data::ChunkQuery& query);

			void injectChunkStorageData(const glm::ivec3& pos, data::ChunkDataRLE&& data);
			bool exportChunkStorageData(const glm::ivec3& pos, data::ChunkDataRLE& data) const;
			bool exportChunkRenderData(const glm::ivec3& pos, data::BlockRegion& data) const;

		private:
			ChunkColumn& getOrCreateColumn(int cx, int cy);
			const ChunkColumn* getColumn(int cx, int cy) const;
			void deleteColumn(int cx, int cy);
			
			Chunk& getOrCreateChunk(const glm::ivec3& cpos);
			Chunk* getChunk(const glm::ivec3& cpos) const;
			void deleteChunk(const glm::ivec3& cpos);
			
			const std::string m_name;

			std::unordered_map<glm::ivec2, ChunkColumn> m_columns;
		};
	}
}