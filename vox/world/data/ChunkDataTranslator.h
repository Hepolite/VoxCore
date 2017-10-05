
#pragma once

#include "vox/world/data/ChunkData.h"

namespace vox
{
	namespace data
	{
		class ChunkDataTranslator
		{
		public:
			ChunkDataFlat toFlat(const ChunkDataRLE& rleData) const;
			ChunkDataRLE toRLE(const ChunkDataFlat& flatData) const;
		};
	}
}