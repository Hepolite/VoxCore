
#include "vox/world/World.h"

#include "vox/events/Chunk.h"
#include "vox/world/Side.h"
#include "vox/world/ChunkSize.h"

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

