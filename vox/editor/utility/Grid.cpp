
#include "vox/editor/utility/Grid.h"

#include "hen/asset/AssetManager.h"
#include "hen/core/Core.h"
#include "hen/render/uniform/UniformBlockManager.h"

#include <glm/gtc/matrix_transform.hpp>

vox::editor::util::Grid::Grid()
{
	m_shader = hen::Core::getAssets().get<hen::shader::ShaderProgram>("editor/grid");
}
vox::editor::util::Grid::~Grid(){}

void vox::editor::util::Grid::onRender(float dt) const
{
	if (!m_visible || m_shader == nullptr || m_mesh == nullptr)
		return;

	hen::Core::getUniformBlockManager().get("Model").setField("transform", glm::translate(glm::mat4{}, m_pos));
	m_shader->bind();
	m_mesh->render();
}

void vox::editor::util::Grid::setAppearance(float size, float resolution)
{
	m_mesh = std::make_unique<hen::render::Mesh<glm::vec2>>();
	m_mesh->setRenderMode(hen::opengl::RenderMode::LINES);
	m_mesh->addAttribute(hen::opengl::Attribute{ 0, hen::opengl::Format::FLOAT, 2, 0 });
	
	auto& vertices = m_mesh->getVertexData();
	auto& indices = m_mesh->getIndiceData();
	for (float i = -0.5f * size; i <= 0.5f * size; i += resolution)
	{
		vertices.emplace_back(i, -0.5f * size);
		vertices.emplace_back(i, 0.5f * size);
		vertices.emplace_back(-0.5f * size, i);
		vertices.emplace_back(0.5f * size, i);

		indices.emplace_back(indices.size());
		indices.emplace_back(indices.size());
		indices.emplace_back(indices.size());
		indices.emplace_back(indices.size());
	}

	m_mesh->build();
}
