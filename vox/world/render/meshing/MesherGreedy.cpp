
#include "vox/world/render/meshing/MesherGreedy.h"

#include "vox/VoxCore.h"
#include "vox/world/ChunkSize.h"
#include "vox/world/render/RenderLayer.h"
#include "vox/world/render/UniverseRenderer.h"

void vox::world::render::MesherGreedy::mesh(ChunkMeshTask& task)
{
	const auto& blocks = VoxCore::getUniverseRenderer().getBlockRenderers();
	const auto& data = task.getData();

	unsigned int indexOffset[RenderLayer::COUNT] = { 0 };
	const float shadowFactors[Side::COUNT] = { 0.95f, 0.8f, 0.9f, 0.75f, 1.0f, 0.7f, 1.0f };

	const Side sides[3] = { Side::NORTH, Side::WEST, Side::TOP };
	glm::uvec3 dim;
	for (dim.x = 0; dim.x < 3; ++dim.x)
	{
		dim.y = (dim.x + 1) % 3;
		dim.z = (dim.x + 2) % 3;

		glm::ivec3 pos;
		for (pos[dim.x] = 0; pos[dim.x] < chunk::SIZE; ++pos[dim.x])
		{
			unsigned long long mask[2][chunk::SIZE][chunk::SIZE] = { 0 };

			for (pos[dim.y] = 0; pos[dim.y] < chunk::SIZE; ++pos[dim.y])
			for (pos[dim.z] = 0; pos[dim.z] < chunk::SIZE; ++pos[dim.z])
			{
				const auto& side = sides[dim.x];
				const auto& sideOther = side.opposite;
				const auto blockdata = data.getBlock(pos);
				const unsigned long long rawdata = blockdata.getData() & data::BlockData::BITMASK_ID;
				const unsigned long long current = blockdata.getId();
				const auto above = data.getBlock(pos + sides[dim.x].z);
				const auto below = data.getBlock(pos - sides[dim.x].z);

				if (current != above.getId() && !blocks[above.getId()].isOccluding(sideOther))
					mask[0][pos[dim.y]][pos[dim.z]] = (rawdata | (above.getData() & data::BlockData::BITMASK_LIGHT)) << 32 | blocks[current].getTexture(side).getTexture(data, pos, side);
				if (current != below.getId() && !blocks[below.getId()].isOccluding(sideOther))
					mask[1][pos[dim.y]][pos[dim.z]] = (rawdata | (below.getData() & data::BlockData::BITMASK_LIGHT)) << 32 | blocks[current].getTexture(sideOther).getTexture(data, pos, sideOther);
			}

			for (unsigned int maskIndex = 0; maskIndex < 2; ++maskIndex)
			for (unsigned int i = 0; i < chunk::SIZE; ++i)
			for (unsigned int j = 0; j < chunk::SIZE; ++j)
			{
				const auto current = mask[maskIndex][i][j];
				if (current == 0)
					continue;

				// Find the size of the face to create
				unsigned int w = 0;
				unsigned int h = 0;
				for (; i + w < chunk::SIZE && mask[maskIndex][i + w][j] == current; ++w);	// Note semicolon at the end
				for (; j + h < chunk::SIZE; ++h)
				for (unsigned int k = i; k < i + w; k++)
					if (mask[maskIndex][k][j + h] != current)
						goto outer;
			outer:
				glm::vec3 size{ 1 };
				size[dim.y] = w;
				size[dim.z] = h;

				// Remove face from mask
				for (unsigned int m = i; m < i + w; m++)
				for (unsigned int n = j; n < j + h; n++)
					mask[maskIndex][m][n] = 0;

				// Add geometry for the face
				pos[dim.y] = i;
				pos[dim.z] = j;
				const auto& side = maskIndex == 0 ? sides[dim.x] : sides[dim.x].opposite;
				const data::BlockData blockdata{ static_cast<unsigned int>(current >> 32) };
				const auto& block = blocks[blockdata.getId()];
				const auto& model = block.getModel();
				const auto& layer = block.getRenderLayer();

				const auto& indices = model.getIndices(side);
				if (indices.size() == 0)
					continue;
				const auto& vertices = model.getVertices(side);

				const auto textureData = static_cast<unsigned int>(current) | (std::hash<glm::ivec4>()(glm::ivec4{ pos, side.id }) << 24);
				for (const auto& vertex : vertices)
					task.getVertices().emplace_back(
						vertex.m_position * size + glm::vec3{ pos },
						vertex.m_normal,
						vertex.m_uv * (dim.x == 0 ? glm::vec3{ w, h, 1 } : glm::vec3{ h, w, 1 }),
						textureData,
						glm::vec4{ 1u + blockdata.getLight() } / 32.0f * glm::vec4{ vertex.m_color * shadowFactors[side.id], 1.0f }
				);
				for (unsigned int index : indices)
					task.getIndices().emplace_back(indexOffset[layer] + index);
				indexOffset[layer] += vertices.size();
			}
		}
	}
}
