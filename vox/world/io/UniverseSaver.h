
#pragma once

#include "vox/world/data/ChunkData.h"
#include "vox/world/io/Region.h"
#include "vox/world/io/WorldHeader.h"
#include "vox/world/Location.h"

#include "hen/io/File.h"

#include <unordered_map>
#include <unordered_set>

namespace vox
{
	namespace io
	{
		class UniverseSaver
		{
		public:
			UniverseSaver();
			~UniverseSaver();

			void onProcess(); // Remove once threading is ready

			void load(world::World* world);

			void storeChunk(const world::Location& location);
			void eraseChunk(const world::Location& location);
			bool readChunk(world::Location& chunk, data::ChunkDataRLE& data);

		private:
			void doWork();
			void finishWork();

			std::unordered_map<world::World*, WorldHeader> m_headers;
			std::unordered_map<world::Location, Region> m_regions;

			std::unordered_set<world::World*> m_worldsToSave;
			std::unordered_set<world::Location> m_regionsToSave;
			std::unordered_set<world::Location> m_regionsToLoad;
		};
	}
}