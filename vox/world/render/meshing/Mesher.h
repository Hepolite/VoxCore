
#pragma once

#include "vox/world/render/meshing/ChunkMeshTask.h"
#include "vox/world/Side.h"

namespace vox
{
	namespace world
	{
		namespace render
		{
			class Mesher
			{
			public:
				Mesher() = default;
				virtual ~Mesher() = default;

				virtual void mesh(ChunkMeshTask& task) = 0;

			protected:
				const float ambient[Side::COUNT] = { 0.05f, 0.05f, 0.05f, 0.05f, 0.05f, 0.05f, 0.05f };
				const float shadows[Side::COUNT] = { 0.90f, 0.75f, 0.85f, 0.70f, 0.95f, 0.65f, 0.95f };
			};
		}
	}
}