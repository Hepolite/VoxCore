
#include "vox/world/World.h"

#include "vox/chunk/Size.h"
#include "vox/world/Side.h"
#include "vox/world/util/RayTrace.h"
#include "vox/world/util/RayBresenham.h"

void vox::world::World::onProcess(float dt)
{
}
void vox::world::World::onLoad(const hen::io::File& file)
{
}
void vox::world::World::onUnload()
{
	m_chunks.clear();
}

vox::world::Chunk* vox::world::World::getChunk(const glm::ivec3& cpos) const
{
	const auto& result = m_chunks.find(cpos);
	return result == m_chunks.end() ? nullptr : result->second.get();
}
std::vector<vox::world::Chunk*> vox::world::World::getChunks() const
{
	std::vector<Chunk*> chunks;
	chunks.reserve(m_chunks.size());
	for (const auto& chunk : m_chunks)
		chunks.push_back(chunk.second.get());
	return chunks;
}
vox::world::Chunk* vox::world::World::createChunk(const glm::ivec3& cpos)
{
	m_chunks.emplace(cpos, std::make_unique<Chunk>(this, cpos));
	Chunk* chunk = getChunk(cpos);
	for (const auto& side : Side::SIDES)
		chunk->setNeighbor(getChunk(cpos + side.z), side);
	return chunk;
}
vox::world::Chunk* vox::world::World::getOrCreateChunk(const glm::ivec3& cpos)
{
	Chunk* chunk = getChunk(cpos);
	return chunk == nullptr ? createChunk(cpos) : chunk;
}
void vox::world::World::deleteChunk(const glm::ivec3& cpos)
{
	Chunk* chunk = getChunk(cpos);
	if (chunk == nullptr)
		return;
	for (const auto& side : Side::SIDES)
		chunk->setNeighbor(nullptr, side);
	m_chunks.erase(cpos);
}

unsigned int vox::world::World::getBlock(const glm::ivec3& pos) const
{
	Chunk* chunk = getChunk(pos >> chunk::SIZE_LG);
	return chunk == nullptr ? 0 : chunk->getBlock(pos & chunk::SIZE_MINUS_ONE);
}
void vox::world::World::setBlock(unsigned int id, const glm::ivec3& pos)
{
	Chunk* chunk = getOrCreateChunk(pos >> chunk::SIZE_LG);
	chunk->setBlock(id, pos & chunk::SIZE_MINUS_ONE);
	if (chunk->isEmpty())
		deleteChunk(chunk->getChunkPos());
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
		Chunk* chunk = getOrCreateChunk(cpos);
		chunk->setBlockCylinder(id, min - cpos * chunk::SIZE, max - cpos * chunk::SIZE, axis);
		if (chunk->isEmpty())
			deleteChunk(chunk->getChunkPos());
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
		Chunk* chunk = getOrCreateChunk(cpos);
		chunk->setBlockEllipse(id, min - cpos * chunk::SIZE, max - cpos * chunk::SIZE);
		if (chunk->isEmpty())
			deleteChunk(chunk->getChunkPos());
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
		Chunk* chunk = getOrCreateChunk(cpos);
		chunk->setBlockRectangle(id, min - cpos * chunk::SIZE, max - cpos * chunk::SIZE);
		if (chunk->isEmpty())
			deleteChunk(chunk->getChunkPos());
	}
}

