
#pragma once

#include "vox/world/ChunkSize.h"

#include <glm/vec3.hpp>

namespace vox
{
	namespace data
	{
		inline unsigned int getIndex(const glm::ivec3& pos)
		{
			return (pos.x * chunk::SIZE + pos.y) * chunk::SIZE + pos.z;
		}
		inline unsigned int getIndex(const glm::ivec3& pos, const glm::ivec3& offset, const glm::ivec3& size)
		{
			return ((pos.x - offset.x) * size.y + (pos.y - offset.y)) * size.z + (pos.z - offset.z);
		}
		inline unsigned int getIndex(const glm::uvec3& pos)
		{
			return (pos.x * chunk::SIZE + pos.y) * chunk::SIZE + pos.z;
		}

		inline glm::ivec3 getPosition(unsigned int index)
		{
			return glm::ivec3{
				index / (vox::chunk::SIZE * vox::chunk::SIZE),
				(index / vox::chunk::SIZE) % vox::chunk::SIZE,
				index % vox::chunk::SIZE
			};
		}
	}
}