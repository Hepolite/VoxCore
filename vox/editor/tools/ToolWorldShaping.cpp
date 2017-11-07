
#include "vox/editor/tools/ToolWorldShaping.h"

#include "hen/util/MathLib.h"

void vox::tool::world::WorldFilterKernel::add(const glm::ivec3& pos, int weight)
{
	m_nodes.emplace_back(pos, weight);
}
void vox::tool::world::WorldFilterKernel::apply(const data::ChunkQuery& source, data::ChunkQuery& target, const glm::ivec3& pos) const
{
	std::unordered_map<unsigned int, int> frequencies;

	const unsigned int currentId = source.get(pos).getId();
	data::BlockData highestData{};
	int highestWeight = std::numeric_limits<int>::min();

	for (const auto& node : m_nodes)
	{
		const auto& location = node.first + pos;
		const auto& weight = node.second;

		if (!source.has(location))
			return;
		const auto id = source.get(location).getId();

		frequencies[id] += weight;
		if (frequencies[id] > highestWeight)
		{
			highestWeight = frequencies[id];
			highestData.setId(id);
		}
	}
	if (currentId != highestData.getId())
		target.add(highestData, pos);
}

vox::data::ChunkQuery vox::tool::world::filter(const data::ChunkQuery& query, const WorldFilterKernel& kernel)
{
	data::ChunkQuery target;
	auto it = query.iter();
	while (it.isValid())
	{
		kernel.apply(query, target, it.getPos());
		it.next();
	}
	return target;
}
vox::data::ChunkQuery vox::tool::world::erode(const data::ChunkQuery& query)
{
	static const glm::ivec3 center{ 0, 0, 0 };
	static const std::vector<glm::ivec3> edges{
		glm::ivec3(-1, 0, 0),
		glm::ivec3(0, -1, 0),
		glm::ivec3(0, 0, -1),
		glm::ivec3(0, 0, 1),
		glm::ivec3(0, 1, 0),
		glm::ivec3(1, 0, 0),
	};
	static const std::vector<glm::ivec3> corners{
		glm::ivec3(-1, -1, 0),
		glm::ivec3(-1, 0, -1),
		glm::ivec3(-1, 0, 1),
		glm::ivec3(-1, 1, 0),
		glm::ivec3(0, -1, -1),
		glm::ivec3(0, -1, 1),
		glm::ivec3(0, 1, -1),
		glm::ivec3(0, 1, 1),
		glm::ivec3(1, -1, 0),
		glm::ivec3(1, 0, -1),
		glm::ivec3(1, 0, 1),
		glm::ivec3(1, 1, 0),
	};
	
	WorldFilterKernel kernel;
	kernel.add(center, -(std::rand() % 4));
	for (const auto& edge : edges)
		kernel.add(edge, std::rand() % 5 + 2);
	for (const auto& corner : corners)
		kernel.add(corner, std::rand() % 5 + 2);
	return filter(query, kernel);
}
vox::data::ChunkQuery vox::tool::world::smooth(const data::ChunkQuery& query)
{
	static const WorldFilterKernel kernel{ {
		{ glm::ivec3(0, 0, 0), 2 },

		{ glm::ivec3(-1, 0, 0), 3 },
		{ glm::ivec3(0, -1, 0), 3 },
		{ glm::ivec3(0, 0, -1), 3 },
		{ glm::ivec3(0, 0, 1), 3 },
		{ glm::ivec3(0, 1, 0), 3 },
		{ glm::ivec3(1, 0, 0), 3 },

		{ glm::ivec3(-1, -1, 0), 1 },
		{ glm::ivec3(-1, 0, -1), 1 },
		{ glm::ivec3(-1, 0, 1), 1 },
		{ glm::ivec3(-1, 1, 0), 1 },
		{ glm::ivec3(0, -1, -1), 1 },
		{ glm::ivec3(0, -1, 1), 1 },
		{ glm::ivec3(0, 1, -1), 1 },
		{ glm::ivec3(0, 1, 1), 1 },
		{ glm::ivec3(1, -1, 0), 1 },
		{ glm::ivec3(1, 0, -1), 1 },
		{ glm::ivec3(1, 0, 1), 1 },
		{ glm::ivec3(1, 1, 0), 1 },
	} };
	return filter(query, kernel);
}

vox::data::ChunkQuery vox::tool::world::pull(const data::ChunkQuery& query, const glm::ivec3& direction)
{
	data::ChunkQuery target;
	auto it = query.iter();
	while (it.isValid())
	{
		const auto pos = it.getPos() + direction;
		const auto data = it.getData();
		it.next();

		if (!query.has(pos) || query.get(pos).getId() != data.getId())
			target.add(data, pos);
	}
	return target;
}
