
#pragma once

#include "vox/world/io/RegionSaver.h"
#include "vox/world/World.h"

#include <glm/gtx/hash.hpp>
#include <glm/vec3.hpp>

#include <memory>
#include <mutex>
#include <thread>
#include <unordered_set>

namespace vox
{
	namespace world
	{
		namespace io
		{
			class WorldSaver
			{
			public:
				WorldSaver() = delete;
				WorldSaver(world::World* world);
				~WorldSaver();

				void markRegionForStorage(const glm::ivec3& region);

			private:
				void workThread();
				void grabWork();
				void doWork();
				void finishWork();

				world::World* m_world;

				std::unordered_set<glm::ivec3> m_regionsToSave;
				std::unique_ptr<RegionSaver> m_saver;

				bool m_running = true;
				std::thread m_thread;
				std::mutex m_mutex;
			};
		}
	}
}