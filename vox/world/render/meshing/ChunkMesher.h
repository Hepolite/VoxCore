
#pragma once

#include "vox/world/render/ChunkRenderer.h"
#include "vox/world/render/meshing/ChunkMeshTask.h"

#include <memory>
#include <mutex>
#include <thread>
#include <unordered_map>

#include <glm/gtx/hash.hpp>
#include <glm/vec3.hpp>

namespace vox
{
	namespace world
	{
		namespace render
		{
			class ChunkMesher
			{
			public:
				ChunkMesher();
				ChunkMesher(const ChunkMesher&) = delete;
				ChunkMesher(ChunkMesher&&) = delete;
				~ChunkMesher();

				ChunkMesher& operator=(const ChunkMesher&) = delete;
				ChunkMesher& operator=(ChunkMesher&&) = delete;

				unsigned int size();
				std::unique_ptr<ChunkRenderer> startTask(ChunkMeshTask&& task);
				bool pollResult(ChunkMeshTask& task);

			private:
				void doWork();
				void clearWork();

				void pushTask(ChunkMeshTask&& task);
				bool pollTask(ChunkMeshTask& task);
				void pushResult(ChunkMeshTask&& task);

				std::unordered_map<glm::ivec3, ChunkMeshTask> m_tasks;
				std::unordered_map<glm::ivec3, ChunkMeshTask> m_products;

				bool m_running = true;
				std::thread m_thread;
				std::mutex m_mutex;
			};
		}
	}
}