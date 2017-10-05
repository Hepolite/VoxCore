
#pragma once

#include "vox/world/data/ChunkQuery.h"

#include "hen/util/MathEnum.h"

#include <glm/vec3.hpp>

namespace vox
{
	namespace data
	{
		namespace BlockQueryHelper
		{
			ChunkReadQuery readBlock(const glm::ivec3& pos);

			ChunkWriteQuery writeBlock(const BlockData& data, const glm::ivec3& pos);
			ChunkWriteQuery writeCylinder(const BlockData& data, const glm::ivec3& start, const glm::ivec3& end, hen::math::Axis axis);
			ChunkWriteQuery writeEllipse(const BlockData& data, const glm::ivec3& start, const glm::ivec3& end);
			ChunkWriteQuery writeLine(const BlockData& data, const glm::ivec3& start, const glm::ivec3& end);
			ChunkWriteQuery writeRectangle(const BlockData& data, const glm::ivec3& start, const glm::ivec3& end);
			ChunkWriteQuery writeSphere(const BlockData& data, const glm::ivec3& center, int radius);
		};
	}
}