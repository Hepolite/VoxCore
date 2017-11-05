
#include "vox/editor/tools/ToolWorldTerraform.h"

void vox::tool::WorldFilterKernel::add(const glm::ivec3& pos, int weight)
{
	m_nodes.emplace_back(pos, weight);
}
void vox::tool::WorldFilterKernel::apply(const data::ChunkQuery& source, data::ChunkQuery& target, const glm::ivec3& pos) const
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
	auto foo = highestData.getId();
	target.add(highestData, pos);
}

vox::data::ChunkQuery vox::tool::generic(const data::ChunkQuery& query, const std::vector<WorldFilterKernel>& kernels)
{
	data::ChunkQuery target;
	auto it = query.iter();
	while (it.isValid())
	{
		const auto pos = it.getPos();
		for (const auto& kernel : kernels)
			kernel.apply(query, target, pos);
		it.next();
	}
	return target;
}

vox::data::ChunkQuery vox::tool::erode(const data::ChunkQuery& query)
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
	if (std::rand() % 3 != 0)
		kernel.add(center, std::rand() % 5);
	for (const auto& edge : edges)
	{
		if (std::rand() % 4 != 0)
			kernel.add(edge, std::rand() % 5 + 2);
	}
	for (const auto& corner : corners)
		kernel.add(corner, std::rand() % 5 + 2);
	return generic(query, { kernel });
}
vox::data::ChunkQuery vox::tool::smooth(const data::ChunkQuery& query)
{
	static const WorldFilterKernel::NodeList nodes{
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
	};
	static const std::vector<WorldFilterKernel> kernels{
		{ nodes },
	};
	return generic(query, kernels);
}
