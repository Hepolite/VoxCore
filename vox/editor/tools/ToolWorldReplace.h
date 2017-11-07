
#pragma once

#include "vox/world/data/BlockData.h"
#include "vox/world/data/ChunkQuery.h"

namespace vox
{
	namespace tool
	{
		namespace world
		{
			data::ChunkQuery replace(const data::ChunkQuery& query, const data::BlockData& replaced, const data::BlockData& replacer);
		}
	}
}