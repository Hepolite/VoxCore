
#pragma once

#include "vox/world/io/WorldLoader.h"
#include "vox/world/io/WorldSaver.h"
#include "vox/world/World.h"

#include "hen/event/Listener.h"

#include <unordered_map>

namespace vox
{
	namespace world
	{
		namespace io
		{
			class UniverseDataFile
			{
			public:
				UniverseDataFile();
				~UniverseDataFile();

				void onProcess();

			private:
				void load(world::World* world);
				void save(world::World* world);
				void close(world::World* world);

				hen::event::Listener m_worldCreateListener;
				hen::event::Listener m_worldDestroyListener;
				hen::event::Listener m_chunkBlockChangeListener;

				std::unordered_map<world::World*, WorldLoader> m_loaders;
				std::unordered_map<world::World*, WorldSaver> m_savers;
			};
		}
	}
}