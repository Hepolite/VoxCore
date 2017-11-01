
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
			ChunkQuery readBlock(const glm::ivec3& pos);
			ChunkQuery readCylinder(const glm::ivec3& start, const glm::ivec3& end, hen::math::Axis axis);
			ChunkQuery readEllipse(const glm::ivec3& start, const glm::ivec3& end);
			ChunkQuery readLine(const glm::ivec3& start, const glm::ivec3& end);
			ChunkQuery readRectangle(const glm::ivec3& start, const glm::ivec3& end);
			ChunkQuery readSphere(const glm::ivec3& center, int radius);

			ChunkQuery writeBlock(const BlockData& data, const glm::ivec3& pos);
			ChunkQuery writeCylinder(const BlockData& data, const glm::ivec3& start, const glm::ivec3& end, hen::math::Axis axis);
			ChunkQuery writeEllipse(const BlockData& data, const glm::ivec3& start, const glm::ivec3& end);
			ChunkQuery writeLine(const BlockData& data, const glm::ivec3& start, const glm::ivec3& end);
			ChunkQuery writeRectangle(const BlockData& data, const glm::ivec3& start, const glm::ivec3& end);
			ChunkQuery writeSphere(const BlockData& data, const glm::ivec3& center, int radius);
		};
	}
}