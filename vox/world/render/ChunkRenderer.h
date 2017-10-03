
#pragma once

#include "vox/world/render/ChunkModelVertex.h"
#include "vox/world/render/RenderLayer.h"

#include "hen/render/Mesh.h"

namespace vox
{
	namespace world
	{
		namespace render
		{
			struct ChunkRenderer
			{
				hen::render::Mesh<ChunkModelVertex> m_mesh;
			};
		}
	}
}