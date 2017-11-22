
#pragma once

#include "vox/world/render/meshing/ChunkMeshTask.h"

namespace vox
{
	namespace world
	{
		namespace render
		{
			class MesherNaive
			{
			public:
				void mesh(ChunkMeshTask& task) const;
			};
		}
	}
}