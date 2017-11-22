
#pragma once

#include "vox/world/render/ChunkModelVertex.h"
#include "vox/world/render/meshing/Mesher.h"
#include "vox/world/Side.h"

#include <vector>

namespace vox
{
	namespace world
	{
		namespace render
		{
			class MesherGreedy : public Mesher
			{
			public:
				virtual void mesh(ChunkMeshTask& task) override final;
			};
		}
	}
}