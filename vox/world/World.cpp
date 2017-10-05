
#include "vox/world/World.h"

#include "vox/events/Chunk.h"
#include "vox/world/data/BlockQueryHelper.h"
#include "vox/world/ChunkSize.h"
#include "vox/world/Side.h"
#include "vox/world/util/RayTrace.h"
#include "vox/world/util/RayBresenham.h"

#include "hen/core/Core.h"
#include "hen/event/EventBus.h"

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

vox::data::BlockData vox::world::World::getBlock(const glm::ivec3& pos) const
{
	if (const auto chunk = getChunk(pos >> chunk::SIZE_LG))
		return chunk->getBlock(pos & chunk::SIZE_MINUS_ONE);
	return 0;
}
void vox::world::World::setBlock(unsigned int id, const glm::ivec3& pos)
{
	auto query = data::BlockQueryHelper::writeBlock(data::BlockData{ id, glm::uvec4{} }, pos);
	acceptQuery(query);
}
void vox::world::World::setBlockCylinder(unsigned int id, const glm::ivec3& start, const glm::ivec3& end, hen::math::Axis axis)
{
	auto query = data::BlockQueryHelper::writeCylinder(data::BlockData{ id, glm::uvec4{} }, start, end, axis);
	acceptQuery(query);
}
void vox::world::World::setBlockEllipse(unsigned int id, const glm::ivec3& start, const glm::ivec3& end)
{
	auto query = data::BlockQueryHelper::writeEllipse(data::BlockData{ id, glm::uvec4{} }, start, end);
	acceptQuery(query);
}
void vox::world::World::setBlockLine(unsigned int id, const glm::ivec3& start, const glm::ivec3& end)
{
	auto query = data::BlockQueryHelper::writeLine(data::BlockData{ id, glm::uvec4{} }, start, end);
	acceptQuery(query);
}
void vox::world::World::setBlockRectangle(unsigned int id, const glm::ivec3& start, const glm::ivec3& end)
{
	auto query = data::BlockQueryHelper::writeRectangle(data::BlockData{ id, glm::uvec4{} }, start, end);
	acceptQuery(query);
}

void vox::world::World::acceptQuery(data::ChunkReadQuery& query) const
{
	for (auto& chunkQuery : query)
	{
		if (const auto chunk = getChunk(chunkQuery.second))
			chunk->acceptQuery(chunkQuery.first);
	}
}
void vox::world::World::acceptQuery(data::ChunkWriteQuery& query)
{
	for (auto& chunkQuery : query)
	{
		const auto cpos = chunkQuery.second;
		auto& blockQuery = chunkQuery.first;
		auto& chunk = getOrCreateChunk(cpos);

		chunk.acceptQuery(blockQuery);
		if (chunk.isEmpty())
			deleteChunk(cpos);
		else
			hen::Core::getEventBus().post(events::ChunkBlockChange{ &chunk, this, cpos, blockQuery.min(), blockQuery.max() });
	}
}

