
#pragma once

#include "vox/world/World.h"

#include <glm/vec3.hpp>

#include <string>

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

			private:
				std::string getFilepath() const;

				world::World* m_world;
				glm::ivec3 m_pos;
			};
		}
	}
}