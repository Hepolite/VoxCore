
#pragma once

#include "vox/world/data/BlockData.h"

#include <glm/vec3.hpp>
#include <vector>

namespace vox
{
	namespace data
	{
		class BlockRegion
		{
		public:
			BlockRegion() = default;
			BlockRegion(const glm::ivec3& offset, const glm::ivec3& size);
			BlockRegion(const glm::ivec3& offset, const glm::ivec3& size, const BlockData& data);
			BlockRegion(const BlockRegion&) = delete;
			BlockRegion(BlockRegion&&) = default;

			BlockRegion& operator=(const BlockRegion&) = delete;
			BlockRegion& operator=(BlockRegion&&) = default;

			inline glm::ivec3 getOffset() const { return m_offset; }
			inline glm::ivec3 getSize() const { return m_size; }

			BlockData getBlock(const glm::ivec3& pos) const;
			void setBlock(const BlockData& block, const glm::ivec3& pos);
			void setBlocks(const BlockData& block, const glm::ivec3& start, const glm::ivec3& end);

		private:
			glm::ivec3 m_size;
			glm::ivec3 m_offset;

			std::vector<BlockData> m_data;
		};
	}
}