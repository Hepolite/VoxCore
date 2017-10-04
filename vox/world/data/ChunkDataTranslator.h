
#pragma once

#include "vox/world/data/ChunkDataFlat.h"
#include "vox/world/data/ChunkDataRLE.h"
#include "vox/world/data/ChunkDataUniform.h"

#include <optional>

namespace vox
{
	namespace data
	{
		class ChunkDataTranslator
		{
		public:
			ChunkDataFlat toFlat(const ChunkDataRLE& data) const;
			ChunkDataFlat toFlat(const ChunkDataUniform& data) const;

			ChunkDataRLE toRLE(const ChunkDataFlat& data) const;
			ChunkDataRLE toRLE(const ChunkDataUniform& data) const;

			std::optional<ChunkDataUniform> toUniform(const ChunkDataFlat& data) const;
			std::optional<ChunkDataUniform> toUniform(const ChunkDataRLE& data) const;
		};
	}
}