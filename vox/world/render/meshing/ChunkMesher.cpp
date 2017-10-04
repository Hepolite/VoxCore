
#include "vox/world/render/meshing/ChunkMesher.h"

#include "vox/VoxCore.h"
#include "vox/world/ChunkSize.h"
#include "vox/world/render/BlockRenderer.h"
#include "vox/world/render/RenderLayer.h"
#include "vox/world/render/UniverseRenderer.h"
#include "vox/world/Side.h"

#include <glm/gtx/hash.hpp>
#include <Log.h>

vox::world::render::ChunkMesher::ChunkMesher()
{
	m_thread = std::thread{ [this]() { doWork(); } };
}
vox::world::render::ChunkMesher::~ChunkMesher()
{
	m_running = false;
	clearWork();
	m_thread.join();
}

void vox::world::render::ChunkMesher::doWork()
{
	while (m_running)
	{
		ChunkMeshTask task;
		while (pollTask(task))
		{
			meshGreedy(task);
			pushResult(std::move(task));
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
}
void vox::world::render::ChunkMesher::clearWork()
{
	std::lock_guard<std::mutex> guard{ m_mutex };
	m_tasks.clear();
	m_products.clear();
}

unsigned int vox::world::render::ChunkMesher::size()
{
	std::lock_guard<std::mutex> guard{ m_mutex };
	return m_tasks.size();
}
void vox::world::render::ChunkMesher::pushTask(ChunkMeshTask&& task)
{
	std::lock_guard<std::mutex> guard{ m_mutex };
	m_tasks.erase(task.getLocation());
	m_tasks.emplace(task.getLocation(), std::move(task));
}
bool vox::world::render::ChunkMesher::pollTask(ChunkMeshTask& task)
{
	std::lock_guard<std::mutex> guard{ m_mutex };
	if (m_tasks.empty())
		return false;
	const auto& begin = m_tasks.begin();
	std::swap(task, begin->second);
	m_tasks.erase(begin->first);
	return true;
}
void vox::world::render::ChunkMesher::pushResult(ChunkMeshTask&& task)
{
	std::lock_guard<std::mutex> guard{ m_mutex };
	m_products.erase(task.getLocation());
	m_products.emplace(task.getLocation(), std::move(task));
}
bool vox::world::render::ChunkMesher::pollResult(ChunkMeshTask& task)
{
	std::lock_guard<std::mutex> guard{ m_mutex };
	if (m_products.empty())
		return false;
	const auto& begin = m_products.begin();
	std::swap(task, begin->second);
	m_products.erase(begin->first);
	return true;
}


std::unique_ptr<vox::world::render::ChunkRenderer> vox::world::render::ChunkMesher::push(const World* world, const glm::ivec3& cpos)
{
	auto chunk = world->getChunk(cpos);
	if (chunk == nullptr)
		return nullptr;
	pushTask(ChunkMeshTask{ Location{ world, cpos }, chunk->getMeshingData() });

	auto renderer = std::make_unique<ChunkRenderer>();
	renderer->m_mesh.addAttribute(hen::opengl::Attribute{ 0, hen::opengl::Format::FLOAT, 3, 0 });
	renderer->m_mesh.addAttribute(hen::opengl::Attribute{ 1, hen::opengl::Format::UNSIGNED_BYTE, 4, 12 });
	renderer->m_mesh.addAttribute(hen::opengl::Attribute{ 2, hen::opengl::Format::UNSIGNED_SHORT, 4, 16 });
	//renderer->m_mesh.addAttribute(hen::opengl::Attribute{ 3, hen::opengl::Format::UNSIGNED_SHORT, 2, 20 });
	renderer->m_mesh.addAttribute(hen::opengl::Attribute{ 4, hen::opengl::Format::UNSIGNED_BYTE, 4, 24 });
	return std::move(renderer);
}
void vox::world::render::ChunkMesher::meshNaive(ChunkMeshTask& task) const
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
void vox::world::render::ChunkMesher::meshGreedy(ChunkMeshTask& task) const
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
				const unsigned long long current = data.getBlock(pos).getId();
				const unsigned long long above = data.getBlock(pos + sides[dim.x].z).getId();
				const unsigned long long below = data.getBlock(pos - sides[dim.x].z).getId();

				if (current != above && !blocks[above].isOccluding(sideOther))
					mask[0][pos[dim.y]][pos[dim.z]] = current << 32 | blocks[current].getTexture(side).getTexture(data, pos, side);
				if (current != below && !blocks[below].isOccluding(sideOther))
					mask[1][pos[dim.y]][pos[dim.z]] = current << 32 | blocks[current].getTexture(sideOther).getTexture(data, pos, sideOther);
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
				const auto& block = blocks[current >> 32];
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
						glm::vec4{ vertex.m_color * shadowFactors[side.id], 1.0f }
				);
				for (unsigned int index : indices)
					task.getIndices().emplace_back(indexOffset[layer] + index);
				indexOffset[layer] += vertices.size();
			}
		}
	}
}
