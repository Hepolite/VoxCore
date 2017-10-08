
#pragma once

#include "vox/world/BlockRegistry.h"
#include "vox/world/Chunk.h"
#include "vox/world/render/BlockRenderer.h"
#include "vox/world/render/WorldRenderer.h"

#include "hen/asset/Ref.h"
#include "hen/event/Listener.h"
#include "hen/render/shader/ShaderProgram.h"
#include "hen/render/texture/Texture2DArray.h"

#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace vox
{
	namespace world
	{
		namespace render
		{
			class UniverseRenderer
			{
			public:
				UniverseRenderer();
				UniverseRenderer(const UniverseRenderer&) = delete;
				UniverseRenderer(UniverseRenderer&&) = delete;
				~UniverseRenderer();

				UniverseRenderer& operator=(const UniverseRenderer&) = delete;
				UniverseRenderer& operator=(UniverseRenderer&&) = delete;

				void onProcess();
				void onRender(float dt) const;
				void onLoad(const BlockRegistry& registry);

				inline const std::vector<BlockRenderer> getBlockRenderers() const { return m_blockRenderers; }

				void setWorldVisibility(const World* world, bool visible);
				bool getWorldVisibility(const World* world) const;

			private:
				void loadRenderers(const BlockRegistry& registry);
				void loadShaders();

				hen::asset::Ref<hen::shader::ShaderProgram> m_shader;
				hen::texture::Texture2DArray m_texture;
				std::vector<BlockRenderer> m_blockRenderers;

				hen::event::Listener m_chunkCreateListener;
				hen::event::Listener m_chunkDestroyListener;
				hen::event::Listener m_chunkBlockChangeListener;

				std::unordered_map<const World*, WorldRenderer> m_renderers;
			};
		}
	}
}