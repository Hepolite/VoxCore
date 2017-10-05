
#include "vox/world/data/BlockQueryHelper.h"

#include "vox/world/ChunkSize.h"
#include "vox/world/util/RayBresenham.h"

#include "hen/util/MathLib.h"

vox::data::ChunkReadQuery vox::data::BlockQueryHelper::readBlock(const glm::ivec3& pos)
{
	ChunkReadQuery chunkQuery;
	BlockReadQuery blockQuery{ true, false, false };
	blockQuery.add(pos & chunk::SIZE_MINUS_ONE);
	chunkQuery.add(std::move(blockQuery), pos >> chunk::SIZE_LG);
	return chunkQuery;
}

vox::data::ChunkWriteQuery vox::data::BlockQueryHelper::writeBlock(const BlockData& data, const glm::ivec3& pos)
{
	ChunkWriteQuery chunkQuery;
	BlockWriteQuery blockQuery{ true, false, false };
	blockQuery.add(data, pos >> chunk::SIZE_LG);
	chunkQuery.add(std::move(blockQuery), pos & chunk::SIZE_MINUS_ONE);
	return chunkQuery;
}
vox::data::ChunkWriteQuery vox::data::BlockQueryHelper::writeLine(const BlockData& data, const glm::ivec3& start, const glm::ivec3& end)
{
	ChunkWriteQuery chunkQuery;
	world::RayBresenham line{ nullptr, start, end };

	glm::ivec3 pos = start >> chunk::SIZE_LG;
	BlockWriteQuery blockQuery{ true, false, false };
	while (line.isValid())
	{
		const glm::ivec3 cpos = line.nextBlockPosition() >> chunk::SIZE_LG;
		const glm::ivec3 bpos = line.getOldBlockPosition() & chunk::SIZE_MINUS_ONE;

		blockQuery.add(data, bpos);
		if (cpos != pos)
		{
			chunkQuery.add(std::move(blockQuery), pos);
			blockQuery = BlockWriteQuery{ true, false, false };
			pos = cpos;
		}
	}
	chunkQuery.add(std::move(blockQuery), pos);

	return chunkQuery;
}
vox::data::ChunkWriteQuery vox::data::BlockQueryHelper::writeRectangle(const BlockData& data, const glm::ivec3& start, const glm::ivec3& end)
{
	ChunkWriteQuery chunkQuery;

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

		BlockWriteQuery blockQuery{ true, false, false };
		blockQuery.add(data, lowest, highest);
		chunkQuery.add(std::move(blockQuery), cpos);

		if (chunkQuery.size() >= MAX_CHUNKS_PER_QUERY)
			return chunkQuery;
	}

	return chunkQuery;
}
