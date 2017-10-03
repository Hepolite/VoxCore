
#pragma once

#include "vox/chunk/ChunkData.h"

namespace vox
{
	namespace chunk
	{
		class ChunkDataUniform : public ChunkData
		{
		public:
			ChunkDataUniform() = default;
			ChunkDataUniform(const BlockData& block) : m_block(block) {}
			virtual ~ChunkDataUniform() = default;

			inline virtual BlockData getBlock(const glm::ivec3& pos) const override final { return m_block; }
			inline virtual void setBlock(const BlockData& block, const glm::ivec3& pos) override final { m_block = block; }

			virtual std::vector<BlockData> expand() const override final;
			virtual void fill(BlockVolume& volume) const override final;

		private:
			BlockData m_block;
		};
	}
}