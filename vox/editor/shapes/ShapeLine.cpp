
#include "vox/editor/shapes/ShapeLine.h"

#include "vox/world/util/RayBresenham.h"

#include "hen/util/MathLib.h"

namespace
{
	void meshBox(const glm::ivec3& start, const glm::ivec3& end, std::vector<glm::vec3>& vertices, std::vector<unsigned int>& indices)
	{
		unsigned int index = vertices.size();
		const auto& min = hen::math::min(start, end);
		const auto& max = hen::math::max(start, end) + 1;

		vertices.emplace_back(min);
		vertices.emplace_back(max.x, min.y, min.z);
		vertices.emplace_back(min.x, max.y, min.z);
		vertices.emplace_back(max.x, max.y, min.z);
		vertices.emplace_back(min.x, min.y, max.z);
		vertices.emplace_back(max.x, min.y, max.z);
		vertices.emplace_back(min.x, max.y, max.z);
		vertices.emplace_back(max);

		indices.emplace_back(index + 0); indices.emplace_back(index + 1);
		indices.emplace_back(index + 2); indices.emplace_back(index + 3);
		indices.emplace_back(index + 4); indices.emplace_back(index + 5);
		indices.emplace_back(index + 6); indices.emplace_back(index + 7);

		indices.emplace_back(index + 0); indices.emplace_back(index + 2);
		indices.emplace_back(index + 1); indices.emplace_back(index + 3);
		indices.emplace_back(index + 4); indices.emplace_back(index + 6);
		indices.emplace_back(index + 5); indices.emplace_back(index + 7);

		indices.emplace_back(index + 0); indices.emplace_back(index + 4);
		indices.emplace_back(index + 1); indices.emplace_back(index + 5);
		indices.emplace_back(index + 2); indices.emplace_back(index + 6);
		indices.emplace_back(index + 3); indices.emplace_back(index + 7);
	}
}

void vox::editor::shape::ShapeLine::mesh(std::vector<glm::vec3>& vertices, std::vector<unsigned int>& indices) const
{
	std::vector<std::pair<glm::ivec3, glm::ivec3>> steps;
	glm::ivec3 start = getStart();
	glm::ivec3 delta;
	glm::ivec3 pos;

	world::RayBresenham trace{ nullptr, getStart(), getEnd() };
	while (trace.isValid())
	{
		const auto nextPos = trace.nextBlockPosition();
		const auto dir = nextPos - pos;
		pos = nextPos;

		if (!trace.isValid() || std::abs(dir.x) + std::abs(dir.y) + std::abs(dir.z) != 1)
		{
			steps.emplace_back(start, start + delta);
			start = pos;
			delta = glm::ivec3{};
		}
		else
			delta += dir;
	}

	for (const auto& step : steps)
		meshBox(step.first, step.second, vertices, indices);
}
