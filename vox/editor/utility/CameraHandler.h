
#pragma once

#include "hen/event/Listener.h"
#include "hen/render/Camera.h"

#include <glm/vec3.hpp>

namespace vox
{
	namespace editor
	{
		namespace util
		{
			class CameraHandler
			{
			public:
				void process(float dt);

				inline glm::vec3 getPosition() const { return m_position; }
				inline glm::vec3 getDirection() const { return m_direction; }
				inline glm::vec3 getVector() const { return m_vector; }

			private:
				void processPersonal(float dt);
				void processOrbital(float dt);

				glm::vec3 m_position;
				glm::vec3 m_direction;
				glm::vec3 m_vector;

				float m_sensitivity = 1.0f;
				float m_distance = 25.0f;
				float m_minDistance = 0.1f;
				float m_maxDistance = 1000.0f;
			};
		}
	}
}