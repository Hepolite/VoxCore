
#pragma once

#include "vox/chunk/ChunkData.h"

#include <vector>

namespace vox
{
	namespace chunk
	{
		class ChunkDataFlat : public ChunkData
		{
		public:
			ChunkDataFlat();
			ChunkDataFlat(std::vector<BlockData>&& data) : m_data(std::move(data)) {}
			virtual ~ChunkDataFlat() = default;

			virtual BlockData getBlock(const glm::ivec3& pos) const override final;
			virtual void setBlock(const BlockData& block, const glm::ivec3& pos) override final;

			inline virtual std::vector<BlockData> expand() const override final { return m_data; }
			virtual void fill(BlockVolume& volume) const override final;

		private:
			std::vector<BlockData> m_data;
		};
	}
}