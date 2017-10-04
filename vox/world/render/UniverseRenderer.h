
#pragma once

#include "vox/world/BlockRegistry.h"
#include "vox/world/Chunk.h"
#include "vox/world/render/BlockRenderer.h"
#include "vox/world/render/meshing/ChunkMesher.h"
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

				void onProcess(float dt);
				void onRender(float dt) const;
				void onLoad(const BlockRegistry& registry);

				inline const std::vector<BlockRenderer> getBlockRenderers() const { return m_blockRenderers; }

				void setWorldVisibility(const World* world, bool visible);

			private:
				void loadRenderers(const BlockRegistry& registry);
				void loadShaders();

				void addMeshTask(const World* world, const glm::ivec3& cpos);
				void handleMeshTasks();
				void finalizeMeshTasks();

				hen::asset::Ref<hen::shader::ShaderProgram> m_shader;
				hen::texture::Texture2DArray m_texture;
				std::vector<BlockRenderer> m_blockRenderers;

				hen::event::Listener m_chunkDestroyListener;
				hen::event::Listener m_chunkBlockChangeListener;

				ChunkMesher m_mesher;
				std::unordered_set<Location> m_meshTasks;

				std::unordered_map<const World*, WorldRenderer> m_renderers;
			};
		}
	}
}