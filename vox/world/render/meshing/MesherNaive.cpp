
#include "vox/world/render/meshing/MesherNaive.h"

#include "vox/VoxCore.h"
#include "vox/world/ChunkSize.h"
#include "vox/world/render/RenderLayer.h"
#include "vox/world/render/UniverseRenderer.h"
#include "vox/world/Side.h"

void vox::world::render::MesherNaive::mesh(ChunkMeshTask& task) const
{
	const auto& blocks = VoxCore::getUniverseRenderer().getBlockRenderers();
	const auto& data = task.getData();

	unsigned int indexOffset[RenderLayer::COUNT] = { 0 };
	const float shadowFactors[Side::COUNT] = { 0.95f, 0.8f, 0.9f, 0.75f, 1.0f, 0.7f, 1.0f };

	glm::ivec3 pos;
	for (pos.x = 0; pos.x < chunk::SIZE; ++pos.x)
	for (pos.y = 0; pos.y < chunk::SIZE; ++pos.y)
	for (pos.z = 0; pos.z < chunk::SIZE; ++pos.z)
	{
		const auto& id = data.getBlock(pos).getId();
		const auto& block = blocks[id];
		const RenderLayer layer = block.getRenderLayer();
		if (layer == RenderLayer::INVISIBLE)
			continue;
		const auto& model = block.getModel();

		for (const auto& side : Side::SIDES)
		{
			const auto& tmpBlock = blocks[data.getBlock(pos + side.z).getId()];
			if (tmpBlock.isOccluding(side.opposite))
				continue;

			const auto& indices = model.getIndices(side);
			if (indices.size() == 0)
				continue;
			const auto& vertices = model.getVertices(side);

			const auto textureData = block.getTexture(side).getTexture(data, pos, side) | (std::hash<glm::ivec4>()(glm::ivec4{ pos, side.id }) << 24);
			for (const auto& vertex : vertices)
				task.getVertices().emplace_back(
					vertex.m_position + glm::vec3{ pos },
					vertex.m_normal,
					vertex.m_uv,
					textureData,
					glm::vec4{ vertex.m_color * shadowFactors[side.id], 1.0f }
			);
			for (unsigned int index : indices)
				task.getIndices().emplace_back(indexOffset[layer] + index);
			indexOffset[layer] += vertices.size();
		}
	}
}
