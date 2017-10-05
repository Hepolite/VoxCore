
#pragma once

namespace vox
{
	namespace region
	{
		constexpr int SIZE_LG = 4;
		constexpr int SIZE = 1 << SIZE_LG;
		constexpr int SIZE_MINUS_ONE = SIZE - 1;

		constexpr int VOLUME = SIZE * SIZE * SIZE;
	}
}