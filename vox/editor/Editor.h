
#pragma once

#include "vox/editor/shapes/Shape.h"
#include "vox/editor/shapes/ShapeBox.h"
#include "vox/editor/shapes/ShapeCylinder.h"
#include "vox/editor/shapes/ShapeEllipse.h"
#include "vox/editor/shapes/ShapeLine.h"
#include "vox/editor/shapes/ShapePoint.h"
#include "vox/editor/utility/CameraHandler.h"
#include "vox/editor/utility/Cursor.h"
#include "vox/editor/utility/Grid.h"

#include "hen/ui/gui/guis/Gui.h"
#include "hen/render/Camera.h"
#include "hen/render/Renderer.h"

#include <functional>
#include <string>

namespace vox
{
	namespace editor
	{
		class Editor
		{
		public:
			Editor();
			Editor(const Editor&) = delete;
			Editor(Editor&&) = delete;
			~Editor();

			Editor& operator=(const Editor&) = delete;
			Editor& operator=(Editor&&) = delete;

			virtual void onProcess(float dt);
			virtual void onRender(float dt) const;

			inline void setShape(shape::Shape* shape) { m_shape = shape; }
			inline shape::Shape* getShape() const { return m_shape; }

		protected:
			util::CameraHandler m_cameraHandler;
			util::Cursor m_cursor;
			util::Grid m_grid;

			void load(const std::string& guiPath);

		private:
			void initScriptData();
			void grabScriptData();

			std::function<void(shape::Shape* shape)> m_inputLeftClick;
			std::function<void(shape::Shape* shape)> m_inputRightClick;

			shape::ShapeBox m_shapeBox;
			shape::ShapeCylinder m_shapeCylinder;
			shape::ShapeEllipse m_shapeEllipse;
			shape::ShapeLine m_shapeLine;
			shape::ShapePoint m_shapePoint;
			shape::Shape* m_shape = nullptr;

			hen::gui::Gui m_gui;
			hen::render::Renderer m_renderer;
		};
	}
}