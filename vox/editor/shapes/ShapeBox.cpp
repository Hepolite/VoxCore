
#include "vox/editor/shapes/ShapeBox.h"

#include "hen/util/MathLib.h"

void vox::editor::shape::ShapeBox::mesh(std::vector<glm::vec3>& vertices, std::vector<unsigned int>& indices) const
{
	const auto& min = hen::math::min(getStart(), getEnd());
	const auto& max = hen::math::max(getStart(), getEnd()) + 1;

	vertices.emplace_back(min);
	vertices.emplace_back(max.x, min.y, min.z);
	vertices.emplace_back(min.x, max.y, min.z);
	vertices.emplace_back(max.x, max.y, min.z);
	vertices.emplace_back(min.x, min.y, max.z);
	vertices.emplace_back(max.x, min.y, max.z);
	vertices.emplace_back(min.x, max.y, max.z);
	vertices.emplace_back(max);

	indices.emplace_back(0); indices.emplace_back(1);
	indices.emplace_back(2); indices.emplace_back(3);
	indices.emplace_back(4); indices.emplace_back(5);
	indices.emplace_back(6); indices.emplace_back(7);

	indices.emplace_back(0); indices.emplace_back(2);
	indices.emplace_back(1); indices.emplace_back(3);
	indices.emplace_back(4); indices.emplace_back(6);
	indices.emplace_back(5); indices.emplace_back(7);

	indices.emplace_back(0); indices.emplace_back(4);
	indices.emplace_back(1); indices.emplace_back(5);
	indices.emplace_back(2); indices.emplace_back(6);
	indices.emplace_back(3); indices.emplace_back(7);
}
