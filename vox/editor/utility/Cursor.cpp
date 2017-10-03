
#include "vox/editor/utility/Cursor.h"

#include "hen/core/Core.h"
#include "hen/ui/Mouse.h"

void vox::editor::util::Cursor::process()
{
	const auto& mouse = hen::Core::getMouse();
	m_mousePos = glm::ivec2{ mouse.getPos() };

	if (!mouse.isHeld(MOUSE_LEFT) && !mouse.isHeld(MOUSE_RIGHT))
		m_clickedPos = m_currentPos;
}

glm::ivec3 vox::editor::util::Cursor::getPos() const
{
	auto delta = m_currentPos - m_clickedPos;
	if (m_lockAxisX) delta.x = 0;
	if (m_lockAxisY) delta.y = 0;
	if (m_lockAxisZ) delta.z = 0;
	return m_clickedPos + delta;
}
