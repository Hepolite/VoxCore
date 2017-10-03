
#pragma once

#include "vox/editor/Editor.h"
#include "vox/world/World.h"

#include "hen/render/Renderer.h"

#include <unordered_map>

namespace vox
{
	namespace editor
	{
		class EditorWorld : public Editor
		{
		public:
			EditorWorld();
			~EditorWorld();

			virtual void onProcess(float dt) override final;
			virtual void onRender(float dt) const override final;

		private:
			world::World* m_world = nullptr;
		};
	}
}