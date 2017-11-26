
#include "vox/world/QueryHelper.h"

#include "vox/world/ChunkSize.h"
#include "vox/world/util/RayBresenham.h"

#include "hen/util/MathLib.h"

namespace
{
	vox::data::BlockData getData(const vox::world::Block& block)
	{
		return vox::data::BlockData{ block.getId(), block.getLightEmittance() };
	}
}

vox::data::ChunkQuery vox::world::QueryHelper::readBlock(const glm::ivec3& pos)
{
	return writeBlock({}, pos);
}
vox::data::ChunkQuery vox::world::QueryHelper::readCylinder(const glm::ivec3& start, const glm::ivec3& end, hen::math::Axis axis)
{
	return writeCylinder({}, start, end, axis);
}
vox::data::ChunkQuery vox::world::QueryHelper::readEllipse(const glm::ivec3& start, const glm::ivec3& end)
{
	return writeEllipse({}, start, end);
}
vox::data::ChunkQuery vox::world::QueryHelper::readLine(const glm::ivec3& start, const glm::ivec3& end)
{
	return writeLine({}, start, end);
}
vox::data::ChunkQuery vox::world::QueryHelper::readRectangle(const glm::ivec3& start, const glm::ivec3& end)
{
	return writeRectangle({}, start, end);
}
vox::data::ChunkQuery vox::world::QueryHelper::readSphere(const glm::ivec3& center, int radius)
{
	return writeSphere({}, center, radius);
}

vox::data::ChunkQuery vox::world::QueryHelper::writeBlock(const world::Block& block, const glm::ivec3& pos)
{
	const auto data = getData(block);

	data::ChunkQuery chunkQuery;
	data::BlockQuery blockQuery;
	blockQuery.add(data, pos & chunk::SIZE_MINUS_ONE);
	chunkQuery.add(std::move(blockQuery), pos >> chunk::SIZE_LG);
	return chunkQuery;
}
vox::data::ChunkQuery vox::world::QueryHelper::writeCylinder(const world::Block& block, const glm::ivec3& start, const glm::ivec3& end, hen::math::Axis axis)
{
	const auto data = getData(block);

	// TODO: This isn't 100% accurate, small cylinders are incorrect
	// The accuracy is good enough for use, however

	data::ChunkQuery chunkQuery;

	const glm::ivec3 min = hen::math::min(start, end);
	const glm::ivec3 max = hen::math::max(start, end);
	const glm::ivec3 cstart = min >> chunk::SIZE_LG;
	const glm::ivec3 cend = max >> chunk::SIZE_LG;

	const auto dim = axis == hen::math::Axis::Z ? glm::uvec2{ 0, 1 } : axis == hen::math::Axis::Y ? glm::uvec2{ 0, 2 } : glm::uvec2{ 1, 2 };
	const auto size = max - min + 1;

	glm::ivec3 cpos;
	for (cpos.x = cstart.x; cpos.x <= cend.x; ++cpos.x)
	for (cpos.y = cstart.y; cpos.y <= cend.y; ++cpos.y)
	for (cpos.z = cstart.z; cpos.z <= cend.z; ++cpos.z)
	{
		data::BlockQuery blockQuery;

		const auto lowest = hen::math::max(min - cpos * chunk::SIZE, glm::ivec3{});
		const auto highest = hen::math::min(max - cpos * chunk::SIZE, glm::ivec3{ chunk::SIZE_MINUS_ONE });

		// Cylinder center is translated to chunk coordinates
		const auto center = glm::vec3{ min - cpos * chunk::SIZE } + 0.5f * glm::vec3{ size };

		glm::ivec3 pos;
		for (pos.x = lowest.x; pos.x <= highest.x; ++pos.x)
		for (pos.y = lowest.y; pos.y <= highest.y; ++pos.y)
		for (pos.z = lowest.z; pos.z <= highest.z; ++pos.z)
		{
			const auto& delta = hen::math::pow(2.0f * (0.5f + glm::vec3{ pos } - center) / glm::vec3{ size }, 2.0f);
			if (delta[dim.x] + delta[dim.y] <= 1.0f)
				blockQuery.add(data, pos);
		}

		if (!chunkQuery.add(std::move(blockQuery), cpos))
			return chunkQuery;
	}

	return chunkQuery;
}
vox::data::ChunkQuery vox::world::QueryHelper::writeEllipse(const world::Block& block, const glm::ivec3& start, const glm::ivec3& end)
{
	const auto data = getData(block);

	data::ChunkQuery chunkQuery;

	const glm::ivec3 min = hen::math::min(start, end);
	const glm::ivec3 max = hen::math::max(start, end);
	const glm::ivec3 cstart = min >> chunk::SIZE_LG;
	const glm::ivec3 cend = max >> chunk::SIZE_LG;

	const auto size = max - min + 1;

	glm::ivec3 cpos;
	for (cpos.x = cstart.x; cpos.x <= cend.x; ++cpos.x)
	for (cpos.y = cstart.y; cpos.y <= cend.y; ++cpos.y)
	for (cpos.z = cstart.z; cpos.z <= cend.z; ++cpos.z)
	{
		data::BlockQuery blockQuery;

		const auto lowest = hen::math::max(min - cpos * chunk::SIZE, glm::ivec3{});
		const auto highest = hen::math::min(max - cpos * chunk::SIZE, glm::ivec3{ chunk::SIZE_MINUS_ONE });

		// Ellipse center is translated to chunk coordinates
		const auto center = glm::vec3{ min - cpos * chunk::SIZE } + 0.5f * glm::vec3{ size };

		glm::ivec3 pos;
		for (pos.x = lowest.x; pos.x <= highest.x; ++pos.x)
		for (pos.y = lowest.y; pos.y <= highest.y; ++pos.y)
		for (pos.z = lowest.z; pos.z <= highest.z; ++pos.z)
		{
			const auto& delta = hen::math::pow(2.0f * (0.5f + glm::vec3{ pos } - center) / glm::vec3{ size }, 2.0f);
			if (delta.x + delta.y + delta.z <= 1.0f)
				blockQuery.add(data, pos);
		}

		if (!chunkQuery.add(std::move(blockQuery), cpos))
			return chunkQuery;
	}

	return chunkQuery;
}
vox::data::ChunkQuery vox::world::QueryHelper::writeLine(const world::Block& block, const glm::ivec3& start, const glm::ivec3& end)
{
	const auto data = getData(block);

	data::ChunkQuery chunkQuery;
	world::RayBresenham line{ nullptr, start, end };

	glm::ivec3 pos = start >> chunk::SIZE_LG;
	data::BlockQuery blockQuery;
	while (line.isValid())
	{
		const glm::ivec3 cpos = line.nextBlockPosition() >> chunk::SIZE_LG;
		const glm::ivec3 bpos = line.getOldBlockPosition() & chunk::SIZE_MINUS_ONE;

		blockQuery.add(data, bpos);
		if (cpos != pos)
		{
			chunkQuery.add(std::move(blockQuery), pos);
			blockQuery = data::BlockQuery{};
			pos = cpos;
		}
	}
	chunkQuery.add(std::move(blockQuery), pos);

	return chunkQuery;
}
vox::data::ChunkQuery vox::world::QueryHelper::writeRectangle(const world::Block& block, const glm::ivec3& start, const glm::ivec3& end)
{
	const auto data = getData(block);

	data::ChunkQuery chunkQuery;

	const glm::ivec3 min = hen::math::min(start, end);
	const glm::ivec3 max = hen::math::max(start, end);
	const glm::ivec3 cstart = min >> chunk::SIZE_LG;
	const glm::ivec3 cend = max >> chunk::SIZE_LG;

	glm::ivec3 cpos;
	for (cpos.x = cstart.x; cpos.x <= cend.x; ++cpos.x)
	for (cpos.y = cstart.y; cpos.y <= cend.y; ++cpos.y)
	for (cpos.z = cstart.z; cpos.z <= cend.z; ++cpos.z)
	{
		const auto lowest = hen::math::max(min - cpos * chunk::SIZE, glm::ivec3{});
		const auto highest = hen::math::min(max - cpos * chunk::SIZE, glm::ivec3{ chunk::SIZE_MINUS_ONE });

		data::BlockQuery blockQuery;
		blockQuery.add(data, lowest, highest);
		if (!chunkQuery.add(std::move(blockQuery), cpos))
			return chunkQuery;
	}

	return chunkQuery;
}
vox::data::ChunkQuery vox::world::QueryHelper::writeSphere(const world::Block& block, const glm::ivec3& center, int radius)
{
	return writeEllipse(block, center - radius, center + radius);
}
