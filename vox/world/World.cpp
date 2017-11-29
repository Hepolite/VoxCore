
#include "vox/world/World.h"

#include "vox/events/Chunk.h"
#include "vox/world/Side.h"
#include "vox/world/ChunkSize.h"
#include "vox/world/util/LightPropagator.h"

#include "hen/core/Core.h"
#include "hen/event/EventBus.h"


std::vector<glm::ivec3> vox::world::World::getChunkCoordinates() const
{
	std::vector<glm::ivec3> coords;
	for (auto& node : m_columns)
	for (auto& coord : node.second)
		coords.emplace_back(node.first, coord.first);
	return coords;
}


vox::world::ChunkColumn& vox::world::World::getOrCreateColumn(int cx, int cy)
{
	return m_columns[glm::ivec2{ cx, cy }];
}
const vox::world::ChunkColumn* vox::world::World::getColumn(int cx, int cy) const
{
	const auto it = m_columns.find(glm::ivec2{ cx, cy });
	return it == m_columns.end() ? nullptr : &it->second;
}
void vox::world::World::deleteColumn(int cx, int cy)
{
	m_columns.erase(glm::ivec2{ cx, cy });
}

vox::world::Chunk& vox::world::World::getOrCreateChunk(const glm::ivec3& cpos)
{
	auto& column = getOrCreateColumn(cpos.x, cpos.y);
	if (auto chunk = column.getChunkAt(cpos.z))
		return *chunk;

	auto& chunk = column.createChunk(cpos);
	for (const auto& side : Side::SIDES)
		chunk.setNeighbor(getChunk(cpos + side.z), side);
	chunk.setNearest(column.getChunkAbove(cpos.z), column.getChunkBelow(cpos.z));

	hen::Core::getEventBus().post(events::ChunkCreate{ this, cpos });

	util::LightPropagator propagator{ &chunk };
	propagator.initialize();
	propagator.work();
	for (auto impacted : propagator.getInpactedChunks())
		hen::Core::getEventBus().post(events::ChunkChange{ this, impacted, impacted->getPos(), propagator.min(), propagator.max() });

	return chunk;
}
vox::world::Chunk* vox::world::World::getChunk(const glm::ivec3& cpos) const
{
	if (auto column = getColumn(cpos.x, cpos.y))
		return column->getChunkAt(cpos.z);
	return nullptr;
}
void vox::world::World::deleteChunk(const glm::ivec3& cpos)
{
	auto chunk = getChunk(cpos);
	if (chunk == nullptr)
		return;
	hen::Core::getEventBus().post(events::ChunkDestroy{ this, cpos });

	for (const auto& side : Side::SIDES)
		chunk->setNeighbor(nullptr, side);
	chunk->setNearest(nullptr, nullptr);

	auto& column = getOrCreateColumn(cpos.x, cpos.y);
	column.deleteChunk(cpos.z);
	if (column.empty())
		deleteColumn(cpos.x, cpos.y);
}


vox::data::BlockData vox::world::World::getBlock(const glm::ivec3& pos) const
{
	if (const auto chunk = getChunk(pos >> chunk::SIZE_LG))
		return chunk->getBlock(pos & chunk::SIZE_MINUS_ONE);
	return data::BlockData{};
}
void vox::world::World::acceptReadQuery(data::ChunkQuery& query) const
{
	for (auto& chunkQuery : query)
	{
		if (const auto chunk = getChunk(chunkQuery.first))
			chunk->acceptReadQuery(chunkQuery.second);
	}
}
void vox::world::World::acceptWriteQuery(data::ChunkQuery& query)
{
	for (auto& chunkQuery : query)
		getOrCreateChunk(chunkQuery.first).acceptWriteQuery(chunkQuery.second);
	for (auto& chunkQuery : query)
	{
		if (auto chunk = getChunk(chunkQuery.first))
		{
			util::LightPropagator propagator{ chunk };
			propagator.work();
			for (auto chunk : propagator.getInpactedChunks())
				hen::Core::getEventBus().post(events::ChunkChange{ this, chunk, chunk->getPos(), propagator.min(), propagator.max() });
		}
	}

	for (auto& chunkQuery : query)
	{
		auto& cpos = chunkQuery.first;
		auto& blockQuery = chunkQuery.second;
		if (const auto chunk = getChunk(cpos))
		{
			if (chunk->empty())
				deleteChunk(cpos);
			else
				hen::Core::getEventBus().post(events::ChunkChange{ this, chunk, cpos, blockQuery.min(), blockQuery.max() });
		}
	}
}

void vox::world::World::injectChunkStorageData(const glm::ivec3& pos, data::ChunkDataRLE&& data)
{
	getOrCreateChunk(pos).injectChunkData(std::move(data));
}
bool vox::world::World::exportChunkStorageData(const glm::ivec3& pos, data::ChunkDataRLE& data) const
{
	const auto chunk = getChunk(pos);
	if (chunk == nullptr || chunk->empty())
		return false;
	data = chunk->getStoringData();
	return true;
}
bool vox::world::World::exportChunkRenderData(const glm::ivec3& pos, data::BlockRegion& data) const
{
	const auto chunk = getChunk(pos);
	if (chunk == nullptr || chunk->empty())
		return false;
	data = chunk->getMeshingData();
	return true;
}
