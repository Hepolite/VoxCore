
#pragma once

#include "vox/world/Chunk.h"
#include "vox/world/data/ChunkQuery.h"

#include "hen/util/MathEnum.h"

#include <glm/gtx/hash.hpp>
#include <glm/vec3.hpp>

#include <memory>
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

			const Chunk* getChunk(const glm::ivec3& cpos) const;
			inline const ChunkMap& getChunks() const { return m_chunks; }

			void setBlock(unsigned int id, const glm::ivec3& pos);
			void setBlockCylinder(unsigned int id, const glm::ivec3& start, const glm::ivec3& end, hen::math::Axis axis);
			void setBlockEllipse(unsigned int id, const glm::ivec3& start, const glm::ivec3& end);
			void setBlockLine(unsigned int id, const glm::ivec3& start, const glm::ivec3& end);
			void setBlockRectangle(unsigned int id, const glm::ivec3& start, const glm::ivec3& end);

			data::BlockData getBlock(const glm::ivec3& pos) const;
			void acceptQuery(data::ChunkReadQuery& query) const;
			void acceptQuery(data::ChunkWriteQuery& query);

		private:
			Chunk* getChunk(const glm::ivec3& cpos);
			Chunk& getOrCreateChunk(const glm::ivec3& cpos);
			void deleteChunk(const glm::ivec3& cpos);

			const std::string m_name;

			ChunkMap m_chunks;
		};
	}
}