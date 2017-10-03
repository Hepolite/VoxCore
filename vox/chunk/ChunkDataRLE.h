
#pragma once

#include "vox/chunk/ChunkData.h"

namespace vox
{
	namespace chunk
	{
		class ChunkDataRLE : public ChunkData
		{
		public:
			ChunkDataRLE() = default;
			ChunkDataRLE(const std::vector<BlockData>& data);
			virtual ~ChunkDataRLE() = default;

			virtual BlockData getBlock(const glm::ivec3& pos) const override final;
			virtual void setBlock(const BlockData& block, const glm::ivec3& pos) override final;

			virtual std::vector<BlockData> expand() const override final;
			virtual void fill(BlockVolume& volume) const override final;

		private:
			struct RLEData
			{
				RLEData(unsigned int endIndex, const BlockData& data) : m_endIndex(endIndex), m_data(data) {}

				unsigned int m_endIndex;
				BlockData m_data;
			};

			std::vector<RLEData> m_data;
		};
	}
}