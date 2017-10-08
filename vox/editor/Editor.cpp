
#include "vox/editor/Editor.h"

#include "hen/core/Core.h"
#include "hen/logic/script/ScriptHelper.h"
#include "hen/render/RenderCore.h"
#include "hen/ui/Mouse.h"

vox::editor::Editor::Editor()
{
	m_renderer = hen::Core::getRenderCore().addRenderer(hen::render::RenderLayer::OPAQUE, [this](float dt) { onRender(dt); });
}
vox::editor::Editor::~Editor()
{
	hen::Core::getRenderCore().deleteRenderer(m_renderer);
}

void vox::editor::Editor::onProcess(float dt)
{
	m_cameraHandler.process(dt);
	m_gui.onProcess(dt);

	m_cursor.process();
	if (m_cursor.hasValidPosition())
	{
		hen::script::ScriptHelper helper{ m_gui.getResources().getScript() };

		auto& mouse = hen::Core::getMouse();
		if (mouse.isReleased(MOUSE_LEFT))
			helper.executeShell(m_inputLeftClick, m_shape);
		if (mouse.isReleased(MOUSE_RIGHT))
			helper.executeShell(m_inputRightClick, m_shape);
	}

	if (m_shape != nullptr)
	{
		if (m_shape->isDynamicScaled())
			m_shape->stretch(m_cursor.getClickedPos(), m_cursor.getPos());
		else
			m_shape->setPos(m_cursor.getCurrentPos());
	}
}
void vox::editor::Editor::onRender(float dt) const
{
	if (m_shape != nullptr && m_cursor.hasValidPosition())
		m_shape->render();
	m_grid.onRender(dt);
}

void vox::editor::Editor::load(const std::string& guiPath)
{
	initScriptData();
	m_gui.onLoad(guiPath);
	grabScriptData();
}

void vox::editor::Editor::initScriptData()
{
	hen::script::ScriptHelper helper{ m_gui.getResources().getScript() };

	helper.addGlobalVariable(this, "Editor");

	helper.addGlobalVariable(&m_cursor, "Cursor");
	helper.addFunction(&util::Cursor::lockAxisX, "lockAxisX");
	helper.addFunction(&util::Cursor::lockAxisY, "lockAxisY");
	helper.addFunction(&util::Cursor::lockAxisZ, "lockAxisZ");

	helper.addGlobalVariable(&m_grid, "Grid");
	helper.addFunction(&util::Grid::setVisible, "setVisible");
	helper.addFunction(&util::Grid::isVisible, "isVisible");

	helper.addGlobalVariable(&m_shapeBox, "ShapeBox");
	helper.addGlobalVariable(&m_shapeCylinder, "ShapeCylinder");
	helper.addGlobalVariable(&m_shapeEllipse, "ShapeEllipse");
	helper.addGlobalVariable(&m_shapeLine, "ShapeLine");
	helper.addGlobalVariable(&m_shapePoint, "ShapePoint");
	helper.addFunction(&Editor::setShape, "setShape");
	helper.addFunction(&Editor::getShape, "getShape");

	helper.addType<shape::Shape>("Shape");
	helper.addRelation<shape::Shape, shape::ShapeBox>();
	helper.addRelation<shape::Shape, shape::ShapeCylinder>();
	helper.addRelation<shape::Shape, shape::ShapeEllipse>();
	helper.addRelation<shape::Shape, shape::ShapeLine>();
	helper.addRelation<shape::Shape, shape::ShapePoint>();
	helper.addFunction(&shape::Shape::getName, "getName");
	helper.addFunction(&shape::Shape::setSizeX, "setSizeX");
	helper.addFunction(&shape::Shape::setSizeY, "setSizeY");
	helper.addFunction(&shape::Shape::setSizeZ, "setSizeZ");
	helper.addFunction(&shape::Shape::isDynamicScaled, "isDynamicScaled");
	helper.addFunction(&shape::Shape::isCentered, "isCentered");
	helper.addFunction(&shape::Shape::setDynamicScaled, "setDynamicScaled");
	helper.addFunction(&shape::Shape::setCentered, "setCentered");
	helper.addFunction(&shape::Shape::getFrom, "getFrom");
	helper.addFunction(&shape::Shape::getTo, "getTo");
	helper.addFunction(&shape::Shape::getPos, "getPos");
	helper.addFunction(&shape::Shape::getSize, "getSize");
	helper.addFunction(&shape::ShapeBox::isHollow, "isHollow");
	helper.addFunction(&shape::ShapeBox::setHollow, "setHollow");
	helper.addFunction(&shape::ShapeCylinder::getAxis, "getAxis");
	helper.addFunction(&shape::ShapeCylinder::setAxis, "setAxis");
	helper.addFunction(&shape::ShapeCylinder::isHollow, "isHollow");
	helper.addFunction(&shape::ShapeCylinder::setHollow, "setHollow");
	helper.addFunction(&shape::ShapeEllipse::isHollow, "isHollow");
	helper.addFunction(&shape::ShapeEllipse::setHollow, "setHollow");
}
void vox::editor::Editor::grabScriptData()
{
	hen::script::ScriptHelper helper{ m_gui.getResources().getScript() };
	
	m_inputLeftClick = helper.getFunction<void, shape::Shape*>("inputLeftClick");
	m_inputRightClick = helper.getFunction<void, shape::Shape*>("inputRightClick");
}
