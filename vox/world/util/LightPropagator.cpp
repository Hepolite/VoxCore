
#include "vox/world/util/LightPropagator.h"

#include "vox/VoxCore.h"
#include "vox/world/BlockRegistry.h"
#include "vox/world/data/Indexing.h"
#include "vox/world/Universe.h"

#include "hen/util/MathLib.h"

void vox::util::LightPropagator::initialize()
{
	auto& data = m_chunk->getActualData();

	if (auto neighbor = m_chunk->getNeighbor(world::Side::NORTH))
	{
		glm::ivec3 pos{ chunk::SIZE_MINUS_ONE, 0, 0 };
		for (pos.y = 0; pos.y < chunk::SIZE; ++pos.y)
		for (pos.z = 0; pos.z < chunk::SIZE; ++pos.z)
			propagateTo(data, neighbor->getBlock(glm::ivec3{ 0, pos.y, pos.z }).getLight(), pos);
	}
	if (auto neighbor = m_chunk->getNeighbor(world::Side::SOUTH))
	{
		glm::ivec3 pos{ 0, 0, 0 };
		for (pos.y = 0; pos.y < chunk::SIZE; ++pos.y)
		for (pos.z = 0; pos.z < chunk::SIZE; ++pos.z)
			propagateTo(data, neighbor->getBlock(glm::ivec3{ chunk::SIZE_MINUS_ONE, pos.y, pos.z }).getLight(), pos);
	}

	if (auto neighbor = m_chunk->getNeighbor(world::Side::WEST))
	{
		glm::ivec3 pos{ 0, chunk::SIZE_MINUS_ONE, 0 };
		for (pos.x = 0; pos.x < chunk::SIZE; ++pos.x)
		for (pos.z = 0; pos.z < chunk::SIZE; ++pos.z)
			propagateTo(data, neighbor->getBlock(glm::ivec3{ pos.x, 0, pos.z }).getLight(), pos);
	}
	if (auto neighbor = m_chunk->getNeighbor(world::Side::EAST))
	{
		glm::ivec3 pos{ 0, 0, 0 };
		for (pos.x = 0; pos.x < chunk::SIZE; ++pos.x)
		for (pos.z = 0; pos.z < chunk::SIZE; ++pos.z)
			propagateTo(data, neighbor->getBlock(glm::ivec3{ pos.x, chunk::SIZE_MINUS_ONE, pos.z }).getLight(), pos);
	}

	if (auto neighbor = m_chunk->getNeighbor(world::Side::TOP))
	{
		glm::ivec3 pos{ 0, 0, chunk::SIZE_MINUS_ONE };
		for (pos.x = 0; pos.x < chunk::SIZE; ++pos.x)
		for (pos.y = 0; pos.y < chunk::SIZE; ++pos.y)
			propagateTo(data, neighbor->getBlock(glm::ivec3{ pos.x, pos.y, 0 }).getLight(), pos);
	}
	if (auto neighbor = m_chunk->getNeighbor(world::Side::BOTTOM))
	{
		glm::ivec3 pos{ 0, 0, 0 };
		for (pos.x = 0; pos.x < chunk::SIZE; ++pos.x)
		for (pos.y = 0; pos.y < chunk::SIZE; ++pos.y)
			propagateTo(data, neighbor->getBlock(glm::ivec3{ pos.x, pos.y, chunk::SIZE_MINUS_ONE }).getLight(), pos);
	}
}
void vox::util::LightPropagator::work()
{
	removal();
	propagate();

	m_impactedChunks.emplace(m_chunk);
}

void vox::util::LightPropagator::propagate()
{
	propagateThisChunk();
	propagateOtherChunks();
}
void vox::util::LightPropagator::propagateThisChunk()
{
	auto& data = m_chunk->getActualData();

	data::Query node;
	while (data.pollPropagationNode(node))
	{
		if ((node.first & data::BlockData::BITMASK_LIGHT) == 0)
			continue;
		const auto pos = data::getPosition(node.second);
		const auto light = node.first.getLight();

		propagateAlong(light, pos, world::Side::NORTH);
		propagateAlong(light, pos, world::Side::WEST);
		propagateAlong(light, pos, world::Side::TOP);

		updateBoundingBox(pos);
	}
}
void vox::util::LightPropagator::propagateOtherChunks()
{
	for (auto chunk : m_chunksToPropagate)
		LightPropagator{ chunk }.propagate();
}
void vox::util::LightPropagator::propagateAlong(glm::uvec4 light, const glm::ivec3& pos, const world::Side& up)
{
	auto& data = m_chunk->getActualData();
	const auto axis = up.axis.z;

	// Spread up
	if (pos[axis] == chunk::SIZE_MINUS_ONE)
	{
		auto target = pos; target[axis] = 0;
		if (auto neighbor = m_chunk->getNeighbor(up))
			propagateTo(neighbor, light, target);
	}
	else
		propagateTo(data, light, pos + up.z);

	// Spread down
	if (pos[axis] == 0)
	{
		// The special rule is that sunlight will *always* spread losslessly downwards
		if (up.id == world::Side::TOP.id)
			light.w++;

		auto target = pos; target[axis] = chunk::SIZE_MINUS_ONE;
		if (auto neighbor = m_chunk->getNeighbor(up.opposite))
			propagateTo(neighbor, light, target);
		else if (auto nearest = m_chunk->getNearestBelow(); nearest != nullptr && up.id == world::Side::TOP.id)
			propagateTo(nearest, glm::uvec4{ 0, 0, 0, light.a }, target);
	}
	else
		propagateTo(data, light, pos - up.z);
}
bool vox::util::LightPropagator::propagateTo(world::Chunk* chunk, const glm::uvec4& light, const glm::ivec3& target)
{
	if (!propagateTo(chunk->getActualData(), light, target))
		return false;
	m_chunksToPropagate.emplace(chunk);
	m_impactedChunks.emplace(chunk);
	return true;
}
bool vox::util::LightPropagator::propagateTo(data::ChunkDataFlat& data, glm::uvec4 light, const glm::ivec3& target)
{
	static auto& registry = VoxCore::getUniverse().getRegistry();

	auto targetData = data.getBlock(target);
	const auto targetLight = targetData.getLight();
	auto& targetBlock = registry.getBlock(targetData.getId());

	const auto reduction = targetBlock.getLightAbsorption() + 1u;
	const auto filter = targetBlock.getLightFilter();
	for (unsigned int i = 0; i < 4; ++i)
	{
		light[i] = light[i] <= reduction[i] ? 0 : light[i] - reduction[i];
		light[i] = hen::math::min(light[i], filter[i]);
		light[i] = light[i] <= targetLight[i] ? 0 : light[i];
	}

	if ((light.r | light.g | light.b | light.a) != 0)
	{
		data.updateBlock(target, targetData.setLight(hen::math::max(targetLight, light)));
		data.pushPropagationNode(target, light);
		return true;
	}
	return false;
}

void vox::util::LightPropagator::removal()
{
	removalThisChunk();
	removalOtherChunks();
}
void vox::util::LightPropagator::removalThisChunk()
{
	auto& data = m_chunk->getActualData();

	data::Query node;
	while (data.pollRemovalNode(node))
	{
		if ((node.first & data::BlockData::BITMASK_LIGHT) == 0)
			continue;
		const auto pos = data::getPosition(node.second);
		const auto light = node.first.getLight();

		removalAlong(light, pos, world::Side::NORTH);
		removalAlong(light, pos, world::Side::WEST);
		removalAlong(light, pos, world::Side::TOP);

		updateBoundingBox(pos);
	}
}
void vox::util::LightPropagator::removalOtherChunks()
{
	for (auto chunk : m_chunksToRemoval)
		LightPropagator{ chunk }.work();
}
void vox::util::LightPropagator::removalAlong(glm::uvec4 light, const glm::ivec3& pos, const world::Side& up)
{
	auto& data = m_chunk->getActualData();
	const auto axis = up.axis.z;

	// Spread up
	if (pos[axis] == chunk::SIZE_MINUS_ONE)
	{
		auto target = pos; target[axis] = 0;
		if (auto neighbor = m_chunk->getNeighbor(up))
			removalTo(neighbor, light, target);
	}
	else
		removalTo(data, light, pos + up.z);

	// Spread down
	if (pos[axis] == 0)
	{
		// The special rule is that sunlight will *always* spread losslessly downwards
		if (up.id == world::Side::TOP.id)
			light.w++;

		auto target = pos; target[axis] = chunk::SIZE_MINUS_ONE;
		if (auto neighbor = m_chunk->getNeighbor(up.opposite))
			removalTo(neighbor, light, target);
		else if (auto nearest = m_chunk->getNearestBelow(); nearest != nullptr && up.id == world::Side::TOP.id)
			removalTo(nearest, glm::uvec4{ 0, 0, 0, light.a }, target);
	}
	else
		removalTo(data, light, pos - up.z);
}
bool vox::util::LightPropagator::removalTo(world::Chunk* chunk, const glm::uvec4& light, const glm::ivec3& target)
{
	if (!removalTo(chunk->getActualData(), light, target))
		return false;
	m_chunksToRemoval.emplace(chunk);
	m_impactedChunks.emplace(chunk);
	return true;
}
bool vox::util::LightPropagator::removalTo(data::ChunkDataFlat& data, const glm::uvec4& light, const glm::ivec3& target)
{
	auto targetData = data.getBlock(target);
	const auto targetLight = targetData.getLight();
	if ((targetData & data::BlockData::BITMASK_LIGHT) == 0)
		return false;
	
	glm::uvec4 propagate;
	glm::uvec4 removal;
	for (unsigned int i = 0; i < 4; ++i)
	{
		if (targetLight[i] == 0)
			continue;

		if (targetLight[i] < light[i])
			removal[i] = targetLight[i];
		else if (targetLight[i] >= light[i])
			propagate[i] = targetLight[i];
	}

	data.updateBlock(target, targetData.setLight(propagate));
	if ((propagate.r | propagate.g | propagate.b | propagate.a) != 0)
		data.pushPropagationNode(target, propagate);
	if ((removal.r | removal.g | removal.b | removal.a) != 0)
	{
		data.pushRemovalNode(target, removal);
		return true;
	}
	return false;
}



void vox::util::LightPropagator::updateBoundingBox(const glm::uvec3& pos)
{
	m_min = hen::math::min(m_min, pos);
	m_max = hen::math::max(m_max, pos);
}
