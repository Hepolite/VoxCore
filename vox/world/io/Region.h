
#pragma once

#include "vox/world/data/ChunkData.h"
#include "vox/world/io/RegionHeader.h"
#include "vox/world/Location.h"

#include <glm/vec3.hpp>

#include <unordered_map>
#include <unordered_set>

namespace vox
{
	namespace io
	{
		class Region
		{
		public:
			Region() = delete;
			Region(const glm::ivec3& pos);

			void saveToDisk();
			void loadFromDisk();

			void save(const world::Location& chunk);
			bool load(const world::Location& chunk, data::ChunkDataRLE& data);

		private:
			RegionHeader m_header;
			glm::ivec3 m_pos;

			std::unordered_map<world::Location, data::ChunkDataRLE> m_dirtyChunks;
			std::unordered_map<world::Location, data::ChunkDataRLE> m_loadedChunks;
		};
	}
}