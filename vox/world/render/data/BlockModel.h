
#pragma once

#include "vox/world/render/BlockModelVertex.h"
#include "vox/world/Side.h"

#include "hen/io/File.h"

#include <vector>

namespace vox
{
	namespace world
	{
		namespace render
		{
			class BlockModel
			{
			public:
				void onLoad(const hen::io::File& file);

				inline const std::vector<BlockModelVertex>& getVertices(Side side) const { return m_vertices[side.id]; }
				inline const std::vector<unsigned int>& getIndices(Side side) const { return m_indices[side.id]; }

			private:
				std::vector<BlockModelVertex> m_vertices[Side::COUNT];
				std::vector<unsigned int> m_indices[Side::COUNT];
			};
		}
	}
}