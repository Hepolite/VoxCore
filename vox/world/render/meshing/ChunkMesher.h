
#pragma once

#include "vox/world/Chunk.h"
#include "vox/world/Location.h"
#include "vox/world/render/ChunkRenderer.h"
#include "vox/world/render/meshing/ChunkMeshTask.h"

#include <memory>
#include <mutex>
#include <thread>
#include <unordered_map>
#include <unordered_set>

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
				std::unique_ptr<ChunkRenderer> push(const World* world, const glm::ivec3& cpos);
				inline bool poll(ChunkMeshTask& result) { return pollResult(result); }

			private:
				void meshNaive(ChunkMeshTask& task) const;
				void meshGreedy(ChunkMeshTask& task) const;

				void doWork();
				void clearWork();

				void pushTask(ChunkMeshTask&& task);
				bool pollTask(ChunkMeshTask& task);
				void pushResult(ChunkMeshTask&& task);
				bool pollResult(ChunkMeshTask& task);

				std::unordered_map<Location, ChunkMeshTask> m_tasks;
				std::unordered_map<Location, ChunkMeshTask> m_products;

				bool m_running = true;
				std::thread m_thread;
				std::mutex m_mutex;
			};
		}
	}
}