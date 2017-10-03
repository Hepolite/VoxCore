
#pragma once

#include "vox/world/Chunk.h"
#include "vox/world/io/RegionFile.h"

#include <glm/vec3.hpp>

#include <unordered_map>

namespace vox
{
	namespace io
	{
		class WorldSaver
		{
		public:
			WorldSaver();
			WorldSaver(const WorldSaver&) = delete;
			WorldSaver(WorldSaver&&) = delete;
			virtual ~WorldSaver();

			WorldSaver& operator=(const WorldSaver&) = delete;
			WorldSaver& operator=(WorldSaver&&) = delete;

			void onProcess();

			void storeChunk(const world::Chunk& chunk);
			void removeChunk(const world::Chunk& chunk);

		private:
			void doWork();
			void clearWork();

			//std::unordered_map<glm::ivec3, RegionFile> m_regions;
		};
	}
}