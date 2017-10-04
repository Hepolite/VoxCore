
#pragma once

#include "vox/world/data/BlockData.h"
#include "vox/world/data/BlockRegion.h"

#include <glm/vec3.hpp>

#include <vector>

namespace vox
{
	namespace data
	{
		class ChunkDataRLE
		{
		public:
			struct RLEData
			{
				RLEData(unsigned int endIndex, const BlockData& data) : m_endIndex(endIndex), m_data(data) {}

				unsigned int m_endIndex;
				BlockData m_data;
			};

			ChunkDataRLE() = default;
			ChunkDataRLE(const BlockData& data) { setBlocks(data); }
			ChunkDataRLE(const BlockRegion& data) { setBlocks(data); }
			ChunkDataRLE(const std::vector<BlockData>& data) { setBlocks(data); }

			void forget();
			
			BlockData getBlock(const glm::ivec3& pos) const;
			BlockRegion getBlocks() const;
			BlockRegion getBlocks(const glm::ivec3& start, const glm::ivec3& end) const;
			void getBlocks(BlockRegion& region, const glm::ivec3& start, const glm::ivec3& end, const glm::ivec3& offset) const;

			void setBlock(const BlockData& block, const glm::ivec3& pos);
			void setBlocks(const BlockData& data);
			void setBlocks(const BlockRegion& data);
			void setBlocks(const std::vector<BlockData>& data);
			
			inline std::vector<RLEData>& getRawData() { return m_data; }
			inline const std::vector<RLEData>& getRawData() const { return m_data; }

		private:
			unsigned int getIndex(const glm::ivec3& pos) const;

			std::vector<RLEData> m_data;
		};
	}
}