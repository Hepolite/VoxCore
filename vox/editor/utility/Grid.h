
#pragma once

#include "hen/asset/Ref.h"
#include "hen/render/Mesh.h"
#include "hen/render/shader/ShaderProgram.h"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <memory>

namespace vox
{
	namespace editor
	{
		namespace util
		{
			class Grid
			{
			public:
				Grid();
				~Grid();

				void onRender(float dt) const;

				void setAppearance(float size, float resolution);

				inline void setPos(const glm::vec3& pos) { m_pos = pos; }
				inline glm::vec3 getPos() const { return m_pos; }

				inline void setVisible(bool visible) { m_visible = visible; }
				inline bool isVisible() const { return m_visible; }

			private:
				hen::asset::Ref<hen::shader::ShaderProgram> m_shader;
				std::unique_ptr<hen::render::Mesh<glm::vec2>> m_mesh;
				glm::vec3 m_pos;

				bool m_visible = true;
			};
		}
	}
}