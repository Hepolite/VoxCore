
#pragma once

#include "vox/world/data/ChunkData.h"
#include "vox/world/World.h"

#include <glm/gtx/hash.hpp>
#include <glm/vec3.hpp>

#include <mutex>
#include <string>
#include <unordered_map>

namespace vox
{
	namespace world
	{
		namespace io
		{
			class RegionLoader
			{
			public:
				RegionLoader() = delete;
				RegionLoader(world::World* world, const glm::ivec3& pos);

				void loadFromDisk();
				bool hasData() const;
				bool pollData(glm::uvec3& pos, data::ChunkDataRLE& data);

			private:
				void pushData(const glm::uvec3& pos, data::ChunkDataRLE&& data);

				std::string getFilepath() const;

				world::World* m_world;
				glm::ivec3 m_pos;

				std::unordered_map<glm::uvec3, data::ChunkDataRLE> m_data;
				bool m_done = false;

				mutable std::mutex m_mutex;
			};
		}
	}
}