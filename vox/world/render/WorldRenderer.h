
#pragma once

#include "vox/world/render/ChunkRenderer.h"
#include "vox/world/render/meshing/ChunkMesher.h"

#include <glm/vec3.hpp>
#include <glm/gtx/hash.hpp>

#include <memory>
#include <unordered_map>

namespace vox
{
	namespace world
	{
		namespace render
		{
			class WorldRenderer
			{
			public:
				WorldRenderer() = default;
				WorldRenderer(const WorldRenderer&) = delete;
				WorldRenderer(WorldRenderer&&) = delete;
				~WorldRenderer() = default;

				WorldRenderer& operator=(const WorldRenderer&) = delete;
				WorldRenderer& operator=(WorldRenderer&&) = delete;

				void onProcess(const World* world);
				void onRender(float dt) const;

				void scheduleMeshTask(const World* world, const glm::ivec3& pos);
				void scheduleMeshRemoval(const glm::ivec3& pos);
				
			private:
				std::unordered_map<glm::ivec3, std::unique_ptr<ChunkRenderer>> m_renderers;
				std::unordered_map<glm::ivec3, std::unique_ptr<ChunkRenderer>> m_replacers;

				ChunkMesher m_mesher;
				std::unordered_set<glm::ivec3> m_meshTasks;
			};
		}
	}
}