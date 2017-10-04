
#pragma once

#include "vox/world/data/BlockData.h"
#include "vox/world/data/BlockRegion.h"

#include <glm/vec3.hpp>

#include <vector>

namespace vox
{
	namespace data
	{
		class ChunkDataFlat
		{
		public:
			ChunkDataFlat() = default;
			ChunkDataFlat(const BlockData& data) { setBlocks(data); }
			ChunkDataFlat(const BlockRegion& data) { setBlocks(data); }
			ChunkDataFlat(const std::vector<BlockData>& data) { setBlocks(data); }
			ChunkDataFlat(std::vector<BlockData>&& data) { setBlocks(data); }

			void forget();
			void expand();

			BlockData getBlock(const glm::ivec3& pos) const;
			BlockRegion getBlocks() const;
			BlockRegion getBlocks(const glm::ivec3& start, const glm::ivec3& end) const;
			void getBlocks(BlockRegion& region, const glm::ivec3& start, const glm::ivec3& end, const glm::ivec3& offset) const;
			
			void setBlock(const BlockData& block, const glm::ivec3& pos);
			void setBlocks(const BlockData& data);
			void setBlocks(const BlockRegion& data);
			void setBlocks(const std::vector<BlockData>& data) { m_data = data; }
			void setBlocks(std::vector<BlockData>&& data) { m_data = std::move(data); }
			
			inline std::vector<BlockData>& getRawData() { return m_data; }
			inline const std::vector<BlockData>& getRawData() const { return m_data; }

		private:
			unsigned int getIndex(const glm::ivec3& pos) const;

			std::vector<BlockData> m_data;
		};
	}
}