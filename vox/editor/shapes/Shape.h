
#pragma once

#include "hen/render/Mesh.h"

#include <glm/vec3.hpp>

#include <string>

namespace vox
{
	namespace editor
	{
		namespace shape
		{
			class Shape
			{
			public:
				Shape(const std::string& name) : m_name(name) {}
				virtual ~Shape() = default;

				void render() const;

				inline std::string getName() { return m_name; }

				void setPos(const glm::ivec3& pos);
				void stretch(const glm::ivec3& start, const glm::ivec3& end);
				inline void setSizeX(int x) { setSize(glm::ivec3{ x, m_size.y, m_size.z }); }
				inline void setSizeY(int y) { setSize(glm::ivec3{ m_size.x, y, m_size.z }); }
				inline void setSizeZ(int z) { setSize(glm::ivec3{ m_size.x, m_size.y, z }); }
				inline glm::ivec3 getFrom() const { return m_start; }
				inline glm::ivec3 getTo() const { return m_end; }
				inline glm::ivec3 getPos() const { return m_pos; }
				inline glm::ivec3 getSize() const { return m_size; }

				inline bool isDynamicScaled() const { return m_isDynamicScaled; }
				inline bool isCentered() const { return m_isCentered; }
				inline void setDynamicScaled(bool dynamicScaled) { m_isDynamicScaled = dynamicScaled; }
				inline void setCentered(bool centered) { m_isCentered = centered; setSize(m_size); }

			protected:
				inline void update() { setSize(m_size); }

				inline glm::ivec3 getStart() const { return m_start - m_pos; }
				inline glm::ivec3 getEnd() const { return m_end - m_pos; }

				virtual void mesh(std::vector<glm::vec3>& vertices, std::vector<unsigned int>& indices) const = 0;

			private:
				void setSize(const glm::ivec3& size);

				void mesh();

				std::string m_name;
				hen::render::Mesh<glm::vec3> m_mesh;

				glm::ivec3 m_start;
				glm::ivec3 m_end;
				glm::ivec3 m_pos;
				glm::ivec3 m_size;

				bool m_isDynamicScaled = true;
				bool m_isCentered = false;
			};
		}
	}
}