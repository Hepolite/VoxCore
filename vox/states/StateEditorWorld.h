
#pragma once

#include "vox/editor/EditorWorld.h"

#include "hen/logic/state/State.h"

#include <memory>

namespace vox
{
	namespace states
	{
		class StateEditorWorld : public hen::state::State
		{
		public:
			StateEditorWorld() = default;
			virtual ~StateEditorWorld() = default;

			virtual void onInitialize() final override;
			virtual void onDeinitialize() final override;
			virtual void onProcess(float dt) final override;

		private:
			std::unique_ptr<editor::EditorWorld> m_editor;
		};
	}
}