
#pragma once

#include "vox/world/io/RegionLoader.h"
#include "vox/world/World.h"

#include <mutex>
#include <thread>
#include <unordered_map>

namespace vox
{
	namespace world
	{
		namespace io
		{
			class WorldLoader
			{
			public:
				WorldLoader() = delete;
				WorldLoader(world::World* world);
				~WorldLoader();

				void onProcess();

				bool isDone() const;

			private:
				void doWork();

				world::World* m_world;

				std::unordered_map<glm::ivec3, RegionLoader> m_regions;
				bool m_done = false;

				std::thread m_thread;
				mutable std::mutex m_mutex;
			};
		}
	}
}