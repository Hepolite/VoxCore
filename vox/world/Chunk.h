
#pragma once

#include "vox/chunk/BlockVolume.h"
#include "vox/chunk/ChunkData.h"
#include "vox/world/Side.h"

#include "hen/util/MathEnum.h"

#include <glm/vec3.hpp>

#include <memory>

namespace vox
{
	namespace world
	{
		class World;

		class Chunk
		{
		public:
			Chunk(World* world, const glm::ivec3& cpos);
			Chunk(const Chunk&) = delete;
			Chunk(Chunk&&) = delete;
			~Chunk();

			Chunk& operator=(const Chunk&) = delete;
			Chunk& operator=(Chunk&&) = delete;

			inline World* getWorld() const { return m_world; }
			inline glm::ivec3 getChunkPos() const { return m_cpos; }

			void setNeighbor(Chunk* neighbor, const Side& side);
			inline Chunk* getNeighbor(const Side& side) const { return m_neighbors[side.id]; }

			void setBlock(unsigned int id, const glm::ivec3& pos);
			void setBlocks(unsigned int id, const std::vector<glm::ivec3>& pos);
			void setBlockCylinder(unsigned int id, const glm::ivec3& start, const glm::ivec3& end, hen::math::Axis axis);
			void setBlockEllipse(unsigned int id, const glm::ivec3& start, const glm::ivec3& end);
			void setBlockRectangle(unsigned int id, const glm::ivec3& start, const glm::ivec3& end);

			unsigned int getBlock(const glm::ivec3& pos) const;
			std::unique_ptr<chunk::BlockVolume> getBlockVolume() const;

			inline bool isEmpty() const { return m_blockCount == 0; }
			inline unsigned int getBlockCount() const { return m_blockCount; }

		private:
			void setBlockUnsafe(unsigned int id, const glm::ivec3& pos);
			unsigned int getBlockUnsafe(const glm::ivec3& pos) const;

			bool preBlockChange(unsigned int id, const glm::ivec3& min, const glm::ivec3& max);
			void postBlockChange(unsigned int id, const glm::ivec3& min, const glm::ivec3& max);

			void initializeBlockData();
			void releaseBlockData();
			void optimizeBlockData();

			World* const m_world;
			glm::ivec3 const m_cpos;

			Chunk* m_neighbors[Side::COUNT] = { nullptr };

			std::unique_ptr<chunk::ChunkData> m_blockData;
			std::vector<chunk::BlockData> m_blockDataExpanded;
			unsigned int m_blockCount = 0;
		};
	}
}