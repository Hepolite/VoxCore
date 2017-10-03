
#pragma once

#include <glm/vec3.hpp>
#include <glm/gtc/packing.hpp>

namespace vox
{
	namespace world
	{
		namespace render
		{
			struct ChunkModelVertex
			{
				ChunkModelVertex(
					const glm::vec3& position,
					const glm::vec3& normal,
					const glm::vec2& uv,
					unsigned int m_textureData,
					const glm::vec4& color) : 
					m_position(position),
					m_normal(glm::packSnorm4x8(glm::vec4{ normal, 0.0f })),
					m_uv(glm::packUnorm2x16(uv / 65535.0f)),
					m_textureData(m_textureData),
					m_color(glm::packUnorm4x8(color))
				{}
				~ChunkModelVertex() = default;

				glm::vec3 m_position;
				unsigned int m_normal;
				unsigned int m_uv;
				unsigned int m_textureData;
				unsigned int m_color;
			};
		}
	}
}