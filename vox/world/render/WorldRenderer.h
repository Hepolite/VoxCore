
#pragma once

#include "vox/world/render/ChunkRenderer.h"

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

				void onRender(float dt) const;

				ChunkRenderer* grab(const glm::ivec3& pos) const;
				void prepare(const glm::ivec3& pos, std::unique_ptr<ChunkRenderer>&& renderer);
				void replace(const glm::ivec3& pos);
				void remove(const glm::ivec3& pos);
				
			private:
				std::unordered_map<glm::ivec3, std::unique_ptr<ChunkRenderer>> m_renderers;
				std::unordered_map<glm::ivec3, std::unique_ptr<ChunkRenderer>> m_replacers;
			};
		}
	}
}