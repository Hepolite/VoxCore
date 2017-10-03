
#pragma once

#include <glm/vec3.hpp>

namespace vox
{
	namespace world
	{
		namespace render
		{
			struct BlockModelVertex
			{
				BlockModelVertex(
					const glm::vec3& position,
					const glm::vec3& normal,
					const glm::vec3& uv,
					const glm::vec3& color)
					: m_position(position), m_normal(normal), m_uv(uv), m_color(color)
				{}
				~BlockModelVertex() = default;

				glm::vec3 m_position;
				glm::vec3 m_normal;
				glm::vec3 m_uv;
				glm::vec3 m_color;
			};
		}
	}
}