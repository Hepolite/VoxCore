
#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace vox
{
	namespace editor
	{
		namespace util
		{
			class Cursor
			{
			public:
				void process();

				inline void setCurrentPos(const glm::ivec3& pos) { m_currentPos = pos; }
				inline glm::ivec3 getCurrentPos() const { return m_currentPos; }
				inline glm::ivec3 getClickedPos() const { return m_clickedPos; }
				inline glm::ivec2 getMousePos() const { return m_mousePos; }
				glm::ivec3 getPos() const;

				inline void setHasValidPosition(bool valid) { m_isValidPosition = valid; }
				inline bool hasValidPosition() const { return m_isValidPosition; }

				inline void lockAxisX(bool locked) { m_lockAxisX = locked; }
				inline void lockAxisY(bool locked) { m_lockAxisY = locked; }
				inline void lockAxisZ(bool locked) { m_lockAxisZ = locked; }
				inline bool isAxisXLocked() const { return m_lockAxisX; }
				inline bool isAxisYLocked() const { return m_lockAxisY; }
				inline bool isAxisZLocked() const { return m_lockAxisZ; }

			private:
				bool m_isValidPosition;
				glm::ivec3 m_clickedPos;
				glm::ivec3 m_currentPos;
				glm::ivec2 m_mousePos;

				bool m_lockAxisX = false;
				bool m_lockAxisY = false;
				bool m_lockAxisZ = false;
			};
		}
	}
}