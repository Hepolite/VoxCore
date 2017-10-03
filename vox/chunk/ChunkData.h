
#pragma once

#include "vox/chunk/BlockData.h"
#include "vox/chunk/BlockVolume.h"
#include "vox/chunk/Size.h"

#include <glm/vec3.hpp>

#include <vector>

namespace vox
{
	namespace chunk
	{
		class ChunkData
		{
		public:
			virtual ~ChunkData() = default;

			virtual BlockData getBlock(const glm::ivec3& pos) const = 0;
			virtual void setBlock(const BlockData& block, const glm::ivec3& pos) = 0;

			virtual std::vector<BlockData> expand() const = 0;
			virtual void fill(BlockVolume& volume) const = 0;

		protected:
			inline unsigned int getIndex(const glm::ivec3& pos) const { return static_cast<unsigned int>((pos.x * SIZE + pos.y) * SIZE + pos.z); }
		};
	}
}