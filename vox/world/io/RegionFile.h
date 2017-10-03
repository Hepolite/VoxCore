
#pragma once

#include <glm/vec3.hpp>

#include <vector>

namespace vox
{
	namespace io
	{
		class RegionFile
		{
		public:
			const static int SIZE_LG = 2;
			const static int SIZE = 1 << SIZE_LG;
			const static int SIZE_BIT = SIZE - 1;

			RegionFile() = default;
			~RegionFile() = default;

			void store(const glm::ivec3& pos, const std::vector<unsigned short>& data);
			std::vector<unsigned short> read(const glm::ivec3& pos) const;

		private:

		};
	}
}