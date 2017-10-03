
#include "vox/editor/shapes/Shape.h"

#include "hen/core/Core.h"
#include "hen/render/uniform/UniformBlockManager.h"
#include "hen/util/MathLib.h"

#include <glm/gtc/matrix_transform.hpp>

void vox::editor::shape::Shape::render() const
{
	hen::Core::getUniformBlockManager().get("Model").setField("transform", glm::translate(glm::mat4{}, glm::vec3{ m_pos }));
	m_mesh.render();
}

void vox::editor::shape::Shape::stretch(const glm::ivec3& start, const glm::ivec3& end)
{
	m_pos = start;
	if (m_isCentered)
	{
		m_start = end;
		m_end = 2 * start - end;
	}
	else
	{
		m_start = start;
		m_end = end;
	}
	m_size = hen::math::abs(m_end - m_start);
	mesh();
}
void vox::editor::shape::Shape::setSize(const glm::ivec3& size)
{
	m_size = hen::math::abs(size);
	if (m_isCentered)
	{
		m_start = m_pos - m_size;
		m_end = m_pos + m_size;
	}
	else
	{
		m_start = m_pos;
		m_end = m_pos + m_size;
	}
	mesh();
}
void vox::editor::shape::Shape::setPos(const glm::ivec3& pos)
{
	if (!m_isDynamicScaled)
	{
		const auto delta = pos - m_pos;
		m_start += delta;
		m_end += delta;
	}
	m_pos = pos;
}

void vox::editor::shape::Shape::mesh()
{
	m_mesh.clear();
	m_mesh.setRenderMode(hen::opengl::RenderMode::LINES);
	m_mesh.addAttribute(hen::opengl::Attribute{ 0, hen::opengl::Format::FLOAT, 3, 0 });
	mesh(m_mesh.getVertexData(), m_mesh.getIndiceData());
	m_mesh.build();
}
