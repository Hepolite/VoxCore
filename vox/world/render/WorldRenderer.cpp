
#include "vox/world/render/WorldRenderer.h"

#include "vox/world/ChunkSize.h"

#include "hen/core/Core.h"
#include "hen/render/uniform/UniformBlockManager.h"

#include <glm/gtc/matrix_transform.hpp>

void vox::world::render::WorldRenderer::onRender(float dt) const
{
	const auto& uboModel = hen::Core::getUniformBlockManager().get("Model");
	for (const auto& renderer : m_renderers)
	{
		uboModel.setField("transform", glm::translate(glm::mat4{}, glm::vec3{ chunk::SIZE * renderer.first }));
		renderer.second->m_mesh.render();
	}
}

vox::world::render::ChunkRenderer* vox::world::render::WorldRenderer::grab(const glm::ivec3& pos) const
{
	const auto& it = m_replacers.find(pos);
	return it == m_replacers.end() ? nullptr : it->second.get();
}
void vox::world::render::WorldRenderer::prepare(const glm::ivec3 & pos, std::unique_ptr<ChunkRenderer>&& renderer)
{
	m_replacers[pos] = std::move(renderer);
}
void vox::world::render::WorldRenderer::replace(const glm::ivec3& pos)
{
	const auto& it = m_replacers.find(pos);
	if (it == m_replacers.end())
		return;
	m_renderers[pos] = std::move(it->second);
	m_replacers.erase(pos);
}
void vox::world::render::WorldRenderer::remove(const glm::ivec3& pos)
{
	m_renderers.erase(pos);
	m_replacers.erase(pos);
}
