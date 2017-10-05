
#include "vox/world/render/WorldRenderer.h"

#include "vox/world/ChunkSize.h"

#include "hen/core/Core.h"
#include "hen/render/uniform/UniformBlockManager.h"

#include <glm/gtc/matrix_transform.hpp>

void vox::world::render::WorldRenderer::onProcess()
{
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
	if (const auto chunk = world->getChunk(pos))
		m_replacers[pos] = m_mesher.startTask(ChunkMeshTask{ pos, chunk->getMeshingData() });
}
void vox::world::render::WorldRenderer::scheduleMeshRemoval(const glm::ivec3& pos)
{
	m_renderers.erase(pos);
	m_replacers.erase(pos);
}
