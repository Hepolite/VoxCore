
#pragma once

#include "vox/world/data/BlockData.h"
#include "vox/world/data/BlockRegion.h"

#include <glm/vec3.hpp>

namespace vox
{
	namespace data
	{
		class ChunkDataUniform
		{
		public:
			ChunkDataUniform() = default;
			ChunkDataUniform(const BlockData& data) : m_data(data) {}

			inline void forget() { m_data = BlockData{}; }

			inline BlockData getBlock() const { return m_data; }
			inline BlockRegion getBlocks() const { return BlockRegion{ m_data }; }
			inline BlockRegion getBlocks(const glm::ivec3& start, const glm::ivec3& end) const { return BlockRegion{ start, end - start + 1, m_data }; }
			
			inline void setBlock(const BlockData& block) { m_data = block; }
			inline void setBlocks(const BlockRegion& region) { m_data = region.getLastBlock(); }

		private:
			BlockData m_data;
		};
	}
}