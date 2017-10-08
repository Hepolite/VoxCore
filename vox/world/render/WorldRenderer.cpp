
#include "vox/world/render/WorldRenderer.h"

#include "vox/world/ChunkSize.h"

#include "hen/core/Core.h"
#include "hen/render/uniform/UniformBlockManager.h"

#include <glm/gtc/matrix_transform.hpp>

void vox::world::render::WorldRenderer::onProcess(const World* world)
{
	while (m_mesher.size() < 50 && !m_meshTasks.empty())
	{
		const auto& pos = *m_meshTasks.begin();

		data::BlockRegion data;
		if (world->exportChunkRenderData(pos, data))
			m_replacers[pos] = m_mesher.startTask(ChunkMeshTask{ pos, std::move(data) });
		m_meshTasks.erase(pos);
	}

	ChunkMeshTask result;
	while (m_mesher.pollResult(result))
	{
		const auto& pos = result.getPos();
		const auto& search = m_replacers.find(pos);
		if (search == m_replacers.end())
			continue;

		auto& replacer = search->second;
		std::swap(replacer->m_mesh.getIndiceData(), result.getIndices());
		std::swap(replacer->m_mesh.getVertexData(), result.getVertices());
		replacer->m_mesh.build();

		std::swap(m_renderers[pos], replacer);
		m_replacers.erase(pos);
	}
}
void vox::world::render::WorldRenderer::onRender(float dt) const
{
	const auto& uboModel = hen::Core::getUniformBlockManager().get("Model");
	for (const auto& renderer : m_renderers)
	{
		uboModel.setField("transform", glm::translate(glm::mat4{}, glm::vec3{ chunk::SIZE * renderer.first }));
		renderer.second->m_mesh.render();
	}
}

void vox::world::render::WorldRenderer::scheduleMeshTask(const World* world, const glm::ivec3& pos)
{
	m_meshTasks.emplace(pos);
}
void vox::world::render::WorldRenderer::scheduleMeshRemoval(const glm::ivec3& pos)
{
	m_meshTasks.erase(pos);
	m_renderers.erase(pos);
	m_replacers.erase(pos);
}
