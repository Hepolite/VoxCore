
#pragma once

#include "vox/world/World.h"

#include <thread>

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

			private:
				void doWork();

				world::World* m_world;

				std::thread m_thread;
			};
		}
	}
}