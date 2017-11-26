
#pragma once

#include "vox/world/Block.h"
#include "vox/world/data/ChunkQuery.h"

#include "hen/util/MathEnum.h"

#include <glm/vec3.hpp>

namespace vox
{
	namespace world
	{
		namespace QueryHelper
		{
			data::ChunkQuery readBlock(const glm::ivec3& pos);
			data::ChunkQuery readCylinder(const glm::ivec3& start, const glm::ivec3& end, hen::math::Axis axis);
			data::ChunkQuery readEllipse(const glm::ivec3& start, const glm::ivec3& end);
			data::ChunkQuery readLine(const glm::ivec3& start, const glm::ivec3& end);
			data::ChunkQuery readRectangle(const glm::ivec3& start, const glm::ivec3& end);
			data::ChunkQuery readSphere(const glm::ivec3& center, int radius);

			data::ChunkQuery writeBlock(const world::Block& block, const glm::ivec3& pos);
			data::ChunkQuery writeCylinder(const world::Block& block, const glm::ivec3& start, const glm::ivec3& end, hen::math::Axis axis);
			data::ChunkQuery writeEllipse(const world::Block& block, const glm::ivec3& start, const glm::ivec3& end);
			data::ChunkQuery writeLine(const world::Block& block, const glm::ivec3& start, const glm::ivec3& end);
			data::ChunkQuery writeRectangle(const world::Block& block, const glm::ivec3& start, const glm::ivec3& end);
			data::ChunkQuery writeSphere(const world::Block& block, const glm::ivec3& center, int radius);
		};
	}
}