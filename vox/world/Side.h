
#pragma once

#include <glm/vec3.hpp>

#include <string>

namespace vox
{
	namespace world
	{
		class Side
		{
		public:
			static const Side NORTH, EAST, SOUTH, WEST, TOP, BOTTOM, UNKNOWN;

			static const unsigned int COUNT = 7;
			static const Side SIDES[COUNT];

			static const Side& fromName(const std::string& name);

			//////////////////////////////////////////////////////////////////

			inline bool operator==(const Side& side) { return id == side.id; }

			const unsigned int id;
			const Side& opposite;
			const glm::ivec3 x;	// Side tangent
			const glm::ivec3 y;	// Side tangent, orthogonal to x
			const glm::ivec3 z;	// Side normal, orthogonal to both x and y

		private:
			Side(unsigned int id, const Side& opposite, const glm::ivec3& x, const glm::ivec3& y, const glm::ivec3& z);
		};
	}
}