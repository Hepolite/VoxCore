
#include "vox/world/Chunk.h"

#include "vox/chunk/ChunkDataFlat.h"
#include "vox/chunk/ChunkDataUniform.h"
#include "vox/chunk/ChunkDataRLE.h"
#include "vox/chunk/Size.h"
#include "vox/events/Chunk.h"

#include "hen/core/Core.h"
#include "hen/event/EventBus.h"
#include "hen/util/MathLib.h"

namespace
{
	inline int getNormalIndex(const glm::ivec3& pos)
	{
		return (pos.x * vox::chunk::SIZE + pos.y) * vox::chunk::SIZE + pos.z;
	}
}


vox::world::Chunk::Chunk(World* world, const glm::ivec3& cpos)
	: m_world(world), m_cpos(cpos)
{
	hen::Core::getEventBus().post(events::ChunkCreate{ world, cpos });
}
vox::world::Chunk::~Chunk()
{
	hen::Core::getEventBus().post(events::ChunkDestroy{ getWorld(), getChunkPos() });
}

void vox::world::Chunk::setNeighbor(Chunk* neighbor, const Side& side)
{
	if (side.id == Side::UNKNOWN.id)
		return;
	if (neighbor == nullptr && m_neighbors[side.id] != nullptr)
		m_neighbors[side.id]->m_neighbors[side.opposite.id] = nullptr;
	m_neighbors[side.id] = neighbor;
	if (neighbor != nullptr)
		neighbor->m_neighbors[side.opposite.id] = this;
}

void vox::world::Chunk::setBlockUnsafe(unsigned int id, const glm::ivec3& pos)
{
	const auto index = getNormalIndex(pos);
	const auto oldId = m_blockDataExpanded[index].m_id;
	if (oldId == 0 && id != 0)
		m_blockCount++;
	else if (oldId != 0 && id == 0)
		m_blockCount--;

	m_blockDataExpanded[index].m_id = id;
}
void vox::world::Chunk::setBlock(unsigned int id, const glm::ivec3& pos)
{
	if (pos.x < 0 || pos.y < 0 || pos.z < 0 || pos.x >= chunk::SIZE || pos.y >= chunk::SIZE || pos.z >= chunk::SIZE)
		return;
	if (preBlockChange(id, pos, pos))
		return;

	setBlockUnsafe(id, pos);

	postBlockChange(id, pos, pos);
}
void vox::world::Chunk::setBlocks(unsigned int id, const std::vector<glm::ivec3>& pos)
{
	glm::ivec3 min{ chunk::SIZE_MINUS_ONE }, max{};
	for (const auto& p : pos)
	{
		if (p.x < 0 || p.y < 0 || p.z < 0 || p.x >= chunk::SIZE || p.y >= chunk::SIZE || p.z >= chunk::SIZE)
			continue;
		min = hen::math::min(min, p);
		max = hen::math::max(max, p);
	}
	if (preBlockChange(id, min, max))
		return;

	for (const auto& p : pos)
	{
		if (p.x < 0 || p.y < 0 || p.z < 0 || p.x >= chunk::SIZE || p.y >= chunk::SIZE || p.z >= chunk::SIZE)
			continue;
		setBlockUnsafe(id, p);
	}

	postBlockChange(id, min, max);
}
void vox::world::Chunk::setBlockCylinder(unsigned int id, const glm::ivec3& start, const glm::ivec3& end, hen::math::Axis axis)
{
	const auto& min = hen::math::max(glm::ivec3{}, start);
	const auto& max = hen::math::min(glm::ivec3{ chunk::SIZE_MINUS_ONE }, end);
	if (preBlockChange(id, min, max))
		return;

	const glm::vec3 size{ hen::math::abs(end - start) + 1 };
	const glm::vec3 center{ -0.5f + glm::vec3{ start } +0.5f * size };
	const auto dim = axis == hen::math::Axis::Z ? glm::uvec2{ 0, 1 } : axis == hen::math::Axis::Y ? glm::uvec2{ 0, 2 } : glm::uvec2{ 1, 2 };

	glm::ivec3 pos;
	for (pos.x = min.x; pos.x <= max.x; ++pos.x)
	for (pos.y = min.y; pos.y <= max.y; ++pos.y)
	for (pos.z = min.z; pos.z <= max.z; ++pos.z)
	{
		// TODO: This is only an approximation; make this better
		const auto& delta = hen::math::pow(2.0f * (glm::vec3{ pos } -center) / size, 2);
		if (delta[dim.x] + delta[dim.y] <= 1.0f)
			setBlockUnsafe(id, pos);
	}

	postBlockChange(id, min, max);
}
void vox::world::Chunk::setBlockEllipse(unsigned int id, const glm::ivec3& start, const glm::ivec3& end)
{
	const auto& min = hen::math::max(glm::ivec3{}, start);
	const auto& max = hen::math::min(glm::ivec3{ chunk::SIZE_MINUS_ONE }, end);
	if (preBlockChange(id, min, max))
		return;

	const glm::vec3 size{ hen::math::abs(end - start) + 1 };
	const glm::vec3 center{ -0.5f + glm::vec3{ start } +0.5f * size };

	glm::ivec3 pos;
	for (pos.x = min.x; pos.x <= max.x; ++pos.x)
	for (pos.y = min.y; pos.y <= max.y; ++pos.y)
	for (pos.z = min.z; pos.z <= max.z; ++pos.z)
	{
		// TODO: This is only an approximation; make this better
		const auto& delta = hen::math::pow(2.0f * (glm::vec3{ pos } -center) / size, 2);
		if (delta.x + delta.y + delta.z <= 1.0f)
			setBlockUnsafe(id, pos);
	}

	postBlockChange(id, min, max);
}
void vox::world::Chunk::setBlockRectangle(unsigned int id, const glm::ivec3& start, const glm::ivec3& end)
{
	const auto& min = hen::math::max(glm::ivec3{}, start);
	const auto& max = hen::math::min(glm::ivec3{ chunk::SIZE_MINUS_ONE }, end);
	if (preBlockChange(id, min, max))
		return;

	glm::ivec3 pos;
	for (pos.x = min.x; pos.x <= max.x; ++pos.x)
	for (pos.y = min.y; pos.y <= max.y; ++pos.y)
	for (pos.z = min.z; pos.z <= max.z; ++pos.z)
		setBlockUnsafe(id, pos);

	postBlockChange(id, min, max);
}

unsigned int vox::world::Chunk::getBlockUnsafe(const glm::ivec3& pos) const
{
	return m_blockData->getBlock(pos).m_id;
}
unsigned int vox::world::Chunk::getBlock(const glm::ivec3& pos) const
{
	if (pos.x < 0 || pos.y < 0 || pos.z < 0 || pos.x >= chunk::SIZE || pos.y >= chunk::SIZE || pos.z >= chunk::SIZE)
		return 0;
	return m_blockData == nullptr ? 0 : getBlockUnsafe(pos);
}
std::unique_ptr<vox::chunk::BlockVolume> vox::world::Chunk::getBlockVolume() const
{
	auto data = std::make_unique<chunk::BlockVolume>(chunk::SIZE + 2, glm::ivec3{ -1 });
	m_blockData->fill(*data);

	glm::ivec3 pos;
	if (const auto& neighbor = getNeighbor(Side::NORTH))
	{
		pos.x = 0;
		for (pos.y = 0; pos.y < chunk::SIZE; ++pos.y)
		for (pos.z = 0; pos.z < chunk::SIZE; ++pos.z)
			data->setBlock(neighbor->getBlockUnsafe(pos), pos + glm::ivec3{ chunk::SIZE, 0, 0 }
		);
	}
	if (const auto& neighbor = getNeighbor(Side::SOUTH))
	{
		pos.x = chunk::SIZE_MINUS_ONE;
		for (pos.y = 0; pos.y < chunk::SIZE; ++pos.y)
		for (pos.z = 0; pos.z < chunk::SIZE; ++pos.z)
			data->setBlock(neighbor->getBlockUnsafe(pos), pos + glm::ivec3{ -chunk::SIZE, 0, 0 }
		);
	}
	if (const auto& neighbor = getNeighbor(Side::WEST))
	{
		pos.y = 0;
		for (pos.x = 0; pos.x < chunk::SIZE; ++pos.x)
		for (pos.z = 0; pos.z < chunk::SIZE; ++pos.z)
			data->setBlock(neighbor->getBlockUnsafe(pos), pos + glm::ivec3{ 0, chunk::SIZE, 0 }
		);
	}
	if (const auto& neighbor = getNeighbor(Side::EAST))
	{
		pos.y = chunk::SIZE_MINUS_ONE;
		for (pos.x = 0; pos.x < chunk::SIZE; ++pos.x)
		for (pos.z = 0; pos.z < chunk::SIZE; ++pos.z)
			data->setBlock(neighbor->getBlockUnsafe(pos), pos + glm::ivec3{ 0, -chunk::SIZE, 0 }
		);
	}
	if (const auto& neighbor = getNeighbor(Side::TOP))
	{
		pos.z = 0;
		for (pos.x = 0; pos.x < chunk::SIZE; ++pos.x)
		for (pos.y = 0; pos.y < chunk::SIZE; ++pos.y)
			data->setBlock(neighbor->getBlockUnsafe(pos), pos + glm::ivec3{ 0, 0, chunk::SIZE }
		);
	}
	if (const auto& neighbor = getNeighbor(Side::BOTTOM))
	{
		pos.z = chunk::SIZE_MINUS_ONE;
		for (pos.x = 0; pos.x < chunk::SIZE; ++pos.x)
		for (pos.y = 0; pos.y < chunk::SIZE; ++pos.y)
			data->setBlock(neighbor->getBlockUnsafe(pos), pos + glm::ivec3{ 0, 0, -chunk::SIZE }
		);
	}
	return data;
}

bool vox::world::Chunk::preBlockChange(unsigned int id, const glm::ivec3& min, const glm::ivec3& max)
{
	if (min.x > max.x || min.y > max.y || min.z > max.z)
		return true;
	if (isEmpty() && id == 0)
		return true;

	if (m_blockData == nullptr)
		initializeBlockData();
	m_blockDataExpanded = m_blockData->expand();
	return false;
}
void vox::world::Chunk::postBlockChange(unsigned int id, const glm::ivec3& min, const glm::ivec3& max)
{
	hen::Core::getEventBus().post(events::ChunkBlockChange{ this, min, max });

	optimizeBlockData();
}

void vox::world::Chunk::initializeBlockData()
{
	m_blockData = std::make_unique<chunk::ChunkDataFlat>();
	m_blockCount = 0;
}
void vox::world::Chunk::releaseBlockData()
{
	m_blockData = nullptr;
	m_blockCount = 0;
}
void vox::world::Chunk::optimizeBlockData()
{
	if (m_blockCount == 0)
		releaseBlockData();

	const auto& first = std::begin(m_blockDataExpanded);
	if (std::equal(first + 1, std::end(m_blockDataExpanded), first))
		m_blockData = std::make_unique<chunk::ChunkDataUniform>(*first);
	else
		m_blockData = std::make_unique<chunk::ChunkDataRLE>(m_blockDataExpanded);
	m_blockDataExpanded.swap(std::vector<chunk::BlockData>{});
}
