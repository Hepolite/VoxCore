
#include "vox/world/render/UniverseRenderer.h"

#include "vox/events/Chunk.h"
#include "vox/world/ChunkSize.h"
#include "vox/world/World.h"

#include "hen/asset/AssetManager.h"
#include "hen/core/Core.h"
#include "hen/event/EventBus.h"

#include <Log.h>

vox::world::render::UniverseRenderer::UniverseRenderer()
{
	auto& eventBus = hen::Core::getEventBus();
	m_chunkCreateListener = eventBus.registerCallback<events::ChunkCreate>([this](events::ChunkCreate& event)
	{
		const auto world = event.getWorld();
		const auto cpos = event.getChunkPos();
		const auto& result = m_renderers.find(world);
		if (result == m_renderers.end())
			return;
		auto& renderer = result->second;

		renderer.scheduleMeshTask(world, cpos);
		renderer.scheduleMeshTask(world, cpos + glm::ivec3{ -1, 0, 0 });
		renderer.scheduleMeshTask(world, cpos + glm::ivec3{ 1, 0, 0 });
		renderer.scheduleMeshTask(world, cpos + glm::ivec3{ 0, -1, 0 });
		renderer.scheduleMeshTask(world, cpos + glm::ivec3{ 0, 1, 0 });
		renderer.scheduleMeshTask(world, cpos + glm::ivec3{ 0, 0, -1 });
		renderer.scheduleMeshTask(world, cpos + glm::ivec3{ 0, 0, 1 });
	});
	m_chunkDestroyListener = eventBus.registerCallback<events::ChunkDestroy>([this](events::ChunkDestroy& event)
	{
		const auto world = event.getWorld();
		const auto cpos = event.getChunkPos();
		const auto& result = m_renderers.find(world);
		if (result == m_renderers.end())
			return;
		auto& renderer = result->second;

		renderer.scheduleMeshTask(world, cpos);
		renderer.scheduleMeshTask(world, cpos + glm::ivec3 { -1, 0, 0 });
		renderer.scheduleMeshTask(world, cpos + glm::ivec3 { 1, 0, 0 });
		renderer.scheduleMeshTask(world, cpos + glm::ivec3 { 0, -1, 0 });
		renderer.scheduleMeshTask(world, cpos + glm::ivec3 { 0, 1, 0 });
		renderer.scheduleMeshTask(world, cpos + glm::ivec3 { 0, 0, -1 });
		renderer.scheduleMeshTask(world, cpos + glm::ivec3 { 0, 0, 1 });
	});
	m_chunkBlockChangeListener = eventBus.registerCallback<events::ChunkBlockChange>([this](events::ChunkBlockChange& event)
	{
		const auto world = event.getWorld();
		const auto cpos = event.getChunkPos();
		const auto& result = m_renderers.find(world);
		if (result == m_renderers.end())
			return;
		auto& renderer = result->second;

		renderer.scheduleMeshTask(world, cpos);
		if (event.getStart().x == 0)
			renderer.scheduleMeshTask(world, cpos + glm::ivec3{ -1, 0, 0 });
		if (event.getEnd().x == chunk::SIZE_MINUS_ONE)
			renderer.scheduleMeshTask(world, cpos + glm::ivec3 { 1, 0, 0 });
		if (event.getStart().y == 0)
			renderer.scheduleMeshTask(world, cpos + glm::ivec3 { 0, -1, 0 });
		if (event.getEnd().y == chunk::SIZE_MINUS_ONE)
			renderer.scheduleMeshTask(world, cpos + glm::ivec3 { 0, 1, 0 });
		if (event.getStart().z == 0)
			renderer.scheduleMeshTask(world, cpos + glm::ivec3 { 0, 0, -1 });
		if (event.getEnd().z == chunk::SIZE_MINUS_ONE)
			renderer.scheduleMeshTask(world, cpos + glm::ivec3 { 0, 0, 1 });
	});
}
vox::world::render::UniverseRenderer::~UniverseRenderer()
{
	auto& eventBus = hen::Core::getEventBus();
	eventBus.unregisterListener(m_chunkCreateListener);
	eventBus.unregisterListener(m_chunkDestroyListener);
	eventBus.unregisterListener(m_chunkBlockChangeListener);
}

void vox::world::render::UniverseRenderer::onProcess()
{
	for (auto& renderer : m_renderers)
		renderer.second.onProcess(renderer.first);
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

void vox::world::render::UniverseRenderer::setWorldVisibility(const World* world, bool visible)
{
	if (visible)
	{
		const auto& it = m_renderers.emplace(std::piecewise_construct, std::make_tuple(world), std::make_tuple());
		if (it.second)
		{
			auto& renderer = it.first->second;
			for (const auto& chunk : world->getChunks())
				renderer.scheduleMeshTask(world, chunk);
		}
	}
	else
		m_renderers.erase(world);
}
