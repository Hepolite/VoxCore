
#include "vox/editor/EditorWorld.h"

#include "vox/VoxCore.h"
#include "vox/world/render/UniverseRenderer.h"
#include "vox/world/Universe.h"
#include "vox/world/World.h"
#include "vox/world/util/RayTrace.h"

#include "hen/core/Core.h"
#include "hen/render/RenderCore.h"
#include "hen/ui/Keyboard.h"

vox::editor::EditorWorld::EditorWorld()
{
	load("data/guis/editor_world.xml");
	m_grid.setAppearance(32.0f, 1.0f);
	auto& camera = hen::Core::getRenderCore().getMainCamera();
	camera.setPosition(glm::vec3{ -10.5f, -10.5f, 15.5f });
	camera.lookTowards(glm::vec3{ 10.5f, 10.5f, 5.5f });

	m_world = VoxCore::getUniverse().getWorld("test");
	VoxCore::getUniverseRenderer().setWorldVisibility(m_world, true);
}
vox::editor::EditorWorld::~EditorWorld()
{
}

void vox::editor::EditorWorld::onProcess(float dt)
{
	VoxCore::getUniverse().onProcess(dt);
	VoxCore::getUniverseRenderer().onProcess();
	Editor::onProcess(dt);

	const auto& keyboard = hen::Core::getKeyboard();
	m_cursor.setHasValidPosition(false);
	world::RayTrace trace{ m_world, m_cameraHandler.getPosition(), m_cameraHandler.getVector(), 1500.0f };
	while (trace.isValid())
	{
		if (trace.nextBlock() != 0)
		{
			const auto& pos = keyboard.isHeld(ALLEGRO_KEY_LSHIFT) ? trace.getOldBlockPosition() : trace.getBlockPosition();
			m_cursor.setHasValidPosition(true);
			m_cursor.setCurrentPos(pos);
			break;
		}
	}
}
void vox::editor::EditorWorld::onRender(float dt) const
{
	VoxCore::getUniverseRenderer().onRender(dt);
	Editor::onRender(dt);
}

/*void vox::editor::EditorWorld::addGuiData()
{
	hen::gui::LoaderButton loader;

	auto& resources = m_gui.getResources();
	auto& blockWindow = resources.getWidget("block_window");
	auto target = &blockWindow;

	const auto& registry = m_universe.getRegistry();
	for (unsigned int i = 0; i < registry.size(); ++i)
	{
		const auto& blockName = registry.getName(i);
		if (blockName.empty())
			continue;

		auto& widget = resources.createWidget("BLOCK_" + blockName, &blockWindow);
		loader.load(widget, {});
		widget.m_link.setTarget(target);
		if (i != 0)
			widget.m_link.setLocation(hen::gui::ComponentLink::Location::BOTTOM_LEFT);
		target = &widget;

		widget.m_asset.addSprite("gui/button_generic_small", "icon");
		widget.m_clickable.setInvoker([this, i]() { setLeftBlockSlot(m_activeBlockSlot, i); });

		hen::render::Text text;
		text.setFont(widget.m_asset.addFont("editor"));
		text.setText(blockName);
		hen::gui::LoaderLabel::setText(widget, text);
	}
}*/
