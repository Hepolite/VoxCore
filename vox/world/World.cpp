
#include "vox/world/World.h"

#include "vox/events/Chunk.h"
#include "vox/world/Side.h"
#include "vox/world/ChunkSize.h"

#include "hen/core/Core.h"
#include "hen/event/EventBus.h"

#include <Log.h>

std::vector<glm::ivec3> vox::world::World::getChunks() const
{
	std::vector<glm::ivec3> chunks;
	for (const auto& pair : m_chunks)
		chunks.push_back(pair.first);
	return chunks;
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
	const auto& data = m_chunks.emplace(std::piecewise_construct, std::make_tuple(cpos), std::make_tuple());
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
		getOrCreateChunk(chunkQuery.second).acceptQuery(chunkQuery.first);

	for (auto& chunkQuery : query)
	{
		const auto cpos = chunkQuery.second;
		auto& blockQuery = chunkQuery.first;
		if (const auto chunk = getChunk(cpos))
		{
			if (chunk->empty())
				deleteChunk(cpos);
			else
				hen::Core::getEventBus().post(events::ChunkBlockChange{ chunk, this, cpos, blockQuery.min(), blockQuery.max() });
		}
	}
}

void vox::world::World::debugMemusage() const
{
	LOG_DEBUG << "====================================";
	LOG_DEBUG << "Memory dump for world " << m_name << ":";
	LOG_DEBUG << "====================================";
	std::unordered_map<glm::ivec3, unsigned int> regions;
	unsigned long long int size = 0;
	unsigned int biggestChunk = 0;
	for (const auto& pair : m_chunks)
	{
		const auto& pos = pair.first;
		const auto& chunk = pair.second;
		size += chunk.memusage();
		biggestChunk = std::max(biggestChunk, chunk.memusage());
		if (regions.find(pos >> 4) == regions.end())
			regions[pos >> 4] = 0;
		regions[pos >> 4] += chunk.memusage();
		LOG_DEBUG << "Chunk [" << pos.x << ", " << pos.y << ", " << pos.z << "] " << chunk.memusage() << "b (" << (float)chunk.memusage() / 1024.0f << "kb)";
	}
	LOG_DEBUG << "====================================";
	unsigned int biggestRegion = 0;
	for (const auto& region : regions)
	{
		LOG_DEBUG << "Region [" << region.first.x << ", " << region.first.y << ", " << region.first.z << "] " << region.second << "b (" << (float)region.second / 1024.0f / 1024.0f << "mb)";
		biggestRegion = std::max(biggestRegion, region.second);
	}
	LOG_DEBUG << "====================================";
	LOG_DEBUG << "Total world memory usage: " << size << "b (" << (float)size / 1024.0f / 1024.0f << "mb)";
	LOG_DEBUG << "Avg. chunk mem usage: " << (float)size / (float)m_chunks.size() << "b (" << (float)size / (float)m_chunks.size() / 1024.0f << "kb), " << m_chunks.size() << " chunks";
	LOG_DEBUG << "Avg. region mem usage: " << (float)size / (float)regions.size() << "b (" << (float)size / (float)regions.size() / 1024.0f / 1024.0f << "mb), " << regions.size() << " regions";
	LOG_DEBUG << "Biggest chunk mem usage: " << biggestChunk << "b (" << (float)biggestChunk / 1024.0f << "kb)";
	LOG_DEBUG << "Biggest region mem usage: " << biggestRegion << "b (" << (float)biggestRegion / 1024.0f / 1024.0f << "mb)";
	LOG_DEBUG << "====================================";
}

