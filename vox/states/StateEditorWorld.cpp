
#include "vox/states/StateEditorWorld.h"

void vox::states::StateEditorWorld::onInitialize()
{
	m_editor = std::make_unique<editor::EditorWorld>();
}
void vox::states::StateEditorWorld::onDeinitialize()
{
}

void vox::states::StateEditorWorld::onProcess(float dt)
{
	m_editor->onProcess(dt);
}
/*void vox::states::StateEditorWorld::onRender(float dt) const
{
	m_editor->onRender(dt);
}*/
