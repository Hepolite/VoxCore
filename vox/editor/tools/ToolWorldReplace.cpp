
#include "vox/editor/tools/ToolWorldReplace.h"

vox::data::ChunkQuery vox::tool::world::replace(const data::ChunkQuery& query, const data::BlockData& replaced, const data::BlockData& replacer)
{
	data::ChunkQuery target;
	auto it = query.iter();
	while (it.isValid())
	{
		const auto data = it.getData();
		if (data.getId() == replaced.getId())
			target.add(replacer, it.getPos());
		it.next();
	}
	return target;
}
