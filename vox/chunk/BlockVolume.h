
#pragma once

#include "vox/chunk/BlockData.h"

#include <glm/vec3.hpp>

#include <vector>

namespace vox
{
	namespace chunk
	{
		class BlockVolume
		{
		public:
			BlockVolume(unsigned int size, const glm::ivec3& offset);
			BlockVolume(const BlockVolume&) = delete;
			BlockVolume(BlockVolume&&) = delete;
			~BlockVolume();

			BlockVolume& operator=(const BlockVolume&) = delete;
			BlockVolume& operator=(BlockVolume&&) = delete;

			BlockData getBlock(const glm::ivec3& pos) const;
			void setBlock(const BlockData& block, const glm::ivec3& pos);

		private:
			unsigned int getIndex(const glm::ivec3& pos) const;

			std::vector<BlockData> m_data;
			const unsigned int m_size;
			const unsigned int m_volume;

			const glm::ivec3 m_offset;
		};
	}
}