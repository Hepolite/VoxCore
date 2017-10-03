
#include "vox/world/render/UniverseRenderer.h"

#include "vox/chunk/Size.h"
#include "vox/events/Chunk.h"

#include "hen/asset/AssetManager.h"
#include "hen/core/Core.h"
#include "hen/event/EventBus.h"

#include <Log.h>

vox::world::render::UniverseRenderer::UniverseRenderer()
{
	auto& eventBus = hen::Core::getEventBus();
	m_chunkDestroyListener = eventBus.registerCallback<events::ChunkDestroy>([this](events::ChunkDestroy& event)
	{
		const auto& renderer = m_renderers.find(event.getWorld());
		if (renderer != m_renderers.end())
			renderer->second.remove(event.getChunkPos());
	});
	m_chunkBlockChangeListener = eventBus.registerCallback<events::ChunkBlockChange>([this](events::ChunkBlockChange& event)
	{
		auto chunk = event.getChunk();
		addMeshTask(chunk);
		if (event.getStart().x == 0)
			addMeshTask(chunk->getNeighbor(Side::SOUTH));
		if (event.getEnd().x == chunk::SIZE_MINUS_ONE)
			addMeshTask(chunk->getNeighbor(Side::NORTH));
		if (event.getStart().y == 0)
			addMeshTask(chunk->getNeighbor(Side::EAST));
		if (event.getEnd().y == chunk::SIZE_MINUS_ONE)
			addMeshTask(chunk->getNeighbor(Side::WEST));
		if (event.getStart().z == 0)
			addMeshTask(chunk->getNeighbor(Side::BOTTOM));
		if (event.getEnd().z == chunk::SIZE_MINUS_ONE)
			addMeshTask(chunk->getNeighbor(Side::TOP));
	});
}
vox::world::render::UniverseRenderer::~UniverseRenderer()
{
	auto& eventBus = hen::Core::getEventBus();
	eventBus.unregisterListener(m_chunkDestroyListener);
	eventBus.unregisterListener(m_chunkBlockChangeListener);
}

void vox::world::render::UniverseRenderer::onProcess(float dt)
{
	handleMeshTasks();
	finalizeMeshTasks();
}
void vox::world::render::UniverseRenderer::onRender(float dt) const
{
	m_texture.bind();
	if (m_shader != nullptr)
		m_shader->bind();
	for (const auto& renderer : m_renderers)
		renderer.second.onRender(dt);
}

void vox::world::render::UniverseRenderer::onLoad(const BlockRegistry& registry)
{
	clock_t startTime = clock();
	
	loadRenderers(registry);
	loadShaders();

	double workTime = (double)(clock() - startTime) / (double)CLOCKS_PER_SEC;
	LOG_INFO << "Spent " << 1000.0 * workTime << "ms loading and building assets";
}
void vox::world::render::UniverseRenderer::loadRenderers(const BlockRegistry& registry)
{
	m_blockRenderers.clear();
	m_blockRenderers.reserve(registry.size());
	for (unsigned int i = 0; i < registry.size(); ++i)
	{
		m_blockRenderers.emplace_back();

		const hen::io::File& file{ "data/universe/blocks/" + registry.getName(i) + ".xml" };
		if (file.exists())
			m_blockRenderers[i].onLoad(file, registry, m_texture);
	}
	m_texture.build();
}
void vox::world::render::UniverseRenderer::loadShaders()
{
	const auto& assets = hen::Core::getAssets();
	m_shader = assets.get<hen::shader::ShaderProgram>("world/chunk_opaque");
}

void vox::world::render::UniverseRenderer::addMeshTask(const Chunk* chunk)
{
	if (chunk != nullptr)
		m_meshTasks.emplace(chunk->getWorld(), chunk->getChunkPos());
}
void vox::world::render::UniverseRenderer::handleMeshTasks()
{
	while (!m_meshTasks.empty() && m_mesher.size() < 50)
	{
		const auto& it = m_meshTasks.begin();
		const auto& task = *it;

		const auto& chunk = task.getWorld()->getChunk(task.getPos());
		const auto& renderer = m_renderers.find(task.getWorld());
		if (chunk != nullptr && renderer != m_renderers.end())
		{
			auto& ptr = m_mesher.mesh(*chunk);
			if (ptr != nullptr)
				renderer->second.prepare(chunk->getChunkPos(), std::move(ptr));
		}

		m_meshTasks.erase(it);
	}
}
void vox::world::render::UniverseRenderer::finalizeMeshTasks()
{
	ChunkMeshTask task;
	while (m_mesher.poll(task))
	{
		const auto& it = m_renderers.find(task.getLocation().getWorld());
		if (it == m_renderers.end())
			continue;
		auto renderer = it->second.grab(task.getLocation().getPos());
		if (renderer == nullptr)
			continue;
		std::swap(task.getIndices(), renderer->m_mesh.getIndiceData());
		std::swap(task.getVertices(), renderer->m_mesh.getVertexData());
		if (renderer->m_mesh.build())
			it->second.replace(task.getLocation().getPos());
		else
			it->second.remove(task.getLocation().getPos());
	}
}

void vox::world::render::UniverseRenderer::setWorldVisibility(const World* world, bool visible)
{
	if (visible)
	{
		const auto& it = m_renderers.emplace(std::piecewise_construct, std::make_tuple(world), std::make_tuple());
		if (it.second)
		{
			for (const auto& chunk : world->getChunks())
				addMeshTask(chunk);
		}
	}
	else
		m_renderers.erase(world);
}
