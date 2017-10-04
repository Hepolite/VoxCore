
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
			BlockRegion();
			BlockRegion(const BlockData& data);
			BlockRegion(const glm::ivec3& offset, const glm::ivec3& size);
			BlockRegion(const glm::ivec3& offset, const glm::ivec3& size, const BlockData& data);

			inline glm::ivec3 getOffset() const { return m_offset; }
			inline glm::ivec3 getSize() const { return m_size; }
			inline unsigned int getVolume() const { return m_size.x * m_size.y * m_size.z; }

			BlockData getLastBlock() const;
			BlockData getBlock(const glm::ivec3& pos) const;
			void setBlock(const BlockData& block, const glm::ivec3& pos);
			void setBlocks(const BlockData& block, const glm::ivec3& start, const glm::ivec3& end);
			void setBlocks(const std::vector<std::pair<BlockData, glm::ivec3>>& data);

			inline std::vector<BlockData>& getRawData() { return m_data; }
			inline const std::vector<BlockData>& getRawData() const { return m_data; }

		private:
			unsigned int getIndex(const glm::ivec3& pos) const;

			const glm::ivec3 m_size;
			const glm::ivec3 m_offset;

			std::vector<BlockData> m_data;
		};
	}
}