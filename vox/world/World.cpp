
#include "vox/world/World.h"

#include "vox/events/Chunk.h"
#include "vox/world/ChunkSize.h"
#include "vox/world/Side.h"
#include "vox/world/util/RayTrace.h"
#include "vox/world/util/RayBresenham.h"

#include "hen/core/Core.h"
#include "hen/event/EventBus.h"

unsigned int vox::world::World::getBlock(const glm::ivec3& pos) const
{
	auto chunk = getChunk(pos >> chunk::SIZE_LG);
	return chunk == nullptr ? 0 : chunk->getBlock(pos & chunk::SIZE_MINUS_ONE).m_data.getId();
}
void vox::world::World::setBlock(unsigned int id, const glm::ivec3& pos)
{
	const auto cpos = pos >> chunk::SIZE_LG;
	const auto bpos = pos & chunk::SIZE_MINUS_ONE;
	const data::BlockQuery query{ data::BlockData{ id,{} }, bpos };
	auto& chunk = getOrCreateChunk(cpos);
	chunk.setBlock(query);
	if (chunk.isEmpty())
		deleteChunk(cpos);
	else
		hen::Core::getEventBus().post(events::ChunkBlockChange{ &chunk, this, cpos, bpos, bpos });
}
void vox::world::World::setBlockCylinder(unsigned int id, const glm::ivec3& start, const glm::ivec3& end, hen::math::Axis axis)
{
	const glm::ivec3 min = hen::math::min(start, end);
	const glm::ivec3 max = hen::math::max(start, end);
	const glm::ivec3 cstart = min >> chunk::SIZE_LG;
	const glm::ivec3 cend = max >> chunk::SIZE_LG;

	glm::ivec3 cpos;
	for (cpos.x = cstart.x; cpos.x <= cend.x; ++cpos.x)
	for (cpos.y = cstart.y; cpos.y <= cend.y; ++cpos.y)
	for (cpos.z = cstart.z; cpos.z <= cend.z; ++cpos.z)
	{
		//Chunk& chunk = getOrCreateChunk(cpos);
		//chunk.setBlockCylinder(id, min - cpos * chunk::SIZE, max - cpos * chunk::SIZE, axis);
	}
}
void vox::world::World::setBlockEllipse(unsigned int id, const glm::ivec3& start, const glm::ivec3& end)
{
	const glm::ivec3 min = hen::math::min(start, end);
	const glm::ivec3 max = hen::math::max(start, end);
	const glm::ivec3 cstart = min >> chunk::SIZE_LG;
	const glm::ivec3 cend = max >> chunk::SIZE_LG;

	glm::ivec3 cpos;
	for (cpos.x = cstart.x; cpos.x <= cend.x; ++cpos.x)
	for (cpos.y = cstart.y; cpos.y <= cend.y; ++cpos.y)
	for (cpos.z = cstart.z; cpos.z <= cend.z; ++cpos.z)
	{
		//Chunk& chunk = getOrCreateChunk(cpos);
		//chunk.setBlockEllipse(id, min - cpos * chunk::SIZE, max - cpos * chunk::SIZE);
	}
}
void vox::world::World::setBlockLine(unsigned int id, const glm::ivec3& start, const glm::ivec3& end)
{
	RayBresenham trace{ this, start, end };
	setBlock(id, trace.getBlockPosition());
	while (trace.isValid())
		setBlock(id, trace.nextBlockPosition());
}
void vox::world::World::setBlockRectangle(unsigned int id, const glm::ivec3& start, const glm::ivec3& end)
{
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
		const auto highest = hen::math::min(max - cpos * chunk::SIZE, glm::ivec3{ chunk::SIZE });
		const data::BlockQueryRectangle query{ data::BlockData{ id,{} }, lowest, highest };
		auto& chunk = getOrCreateChunk(cpos);
		chunk.setBlocks(query);
		if (chunk.isEmpty())
			deleteChunk(cpos);
		else
			hen::Core::getEventBus().post(events::ChunkBlockChange{ &chunk, this, cpos, lowest, highest });
	}
}

const vox::world::Chunk* vox::world::World::getChunk(const glm::ivec3& cpos) const
{
	const auto& loc = m_chunks.find(cpos);
	return loc == m_chunks.end() ? nullptr : &loc->second;
}
vox::world::Chunk* vox::world::World::getChunk(const glm::ivec3& cpos)
{
	const auto& loc = m_chunks.find(cpos);
	return loc == m_chunks.end() ? nullptr : &loc->second;
}
vox::world::Chunk& vox::world::World::getOrCreateChunk(const glm::ivec3& cpos)
{
	const auto& data = m_chunks.emplace(cpos, Chunk{});
	if (data.second)
		hen::Core::getEventBus().post(events::ChunkCreate{ this, cpos });
	auto& chunk = data.first->second;
	for (const auto& side : Side::SIDES)
		chunk.setNeighbor(getChunk(cpos + side.z), side);
	return chunk;
}
void vox::world::World::deleteChunk(const glm::ivec3& cpos)
{
	auto chunk = getChunk(cpos);
	if (chunk == nullptr)
		return;
	for (const auto& side : Side::SIDES)
		chunk->setNeighbor(nullptr, side);
	hen::Core::getEventBus().post(events::ChunkDestroy{ this, cpos });
	m_chunks.erase(cpos);
}

