
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
	std::vector<glm::ivec3> steps;
	steps.emplace_back(getStart());
	steps.emplace_back(getStart());

	world::RayBresenham trace{ nullptr, getStart(), getEnd() };
	while (trace.isValid())
	{
		const auto oldPos = trace.getBlockPosition();
		const auto pos = trace.nextBlockPosition();
		const auto delta = pos - oldPos;

		if (std::abs(delta.x) + std::abs(delta.y) + std::abs(delta.z) != 1)
		{
			steps.emplace_back(pos);
			steps.emplace_back(pos);
		}
		else
			steps.back() += delta;
	}

	for (unsigned int i = 0; i + 1 < steps.size(); i += 2)
		meshBox(steps[i], steps[i + 1], vertices, indices);
}
