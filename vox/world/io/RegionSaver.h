
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
			class RegionSaver
			{
			public:
				RegionSaver() = delete;
				RegionSaver(const world::World* world, const glm::ivec3& pos);

				void saveToDisk();

			private:
				std::string getFilepath() const;

				const world::World* m_world;
				glm::ivec3 m_pos;
			};
		}
	}
}