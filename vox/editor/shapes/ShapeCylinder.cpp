
#include "vox/editor/shapes/ShapeCylinder.h"

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
	void meshPart(const glm::ivec3& offset, const glm::ivec2& pos, const glm::ivec2& mod, std::vector<glm::vec3>& vertices, std::vector<unsigned int>& indices)
	{
		const auto v1 = offset + glm::ivec3{ pos.x + mod.x, pos.y + mod.y, 0 };
		const auto v2 = offset + glm::ivec3{ -pos.x, pos.y + mod.y, 0 };
		const auto v3 = offset + glm::ivec3{ pos.x + mod.x, -pos.y, 0 };
		const auto v4 = offset + glm::ivec3{ -pos.x, -pos.y, 0 };

		meshBox(v1, v1, vertices, indices);
		meshBox(v2, v2, vertices, indices);
		meshBox(v3, v3, vertices, indices);
		meshBox(v4, v4, vertices, indices);
	}

	std::vector<glm::ivec2> getQuarterCylinderCoordinates(const glm::ivec2& size)
	{
		const glm::ivec2 r = size * size;
		const glm::ivec2 fr = 4 * r;

		std::vector<glm::ivec2> coords, secondSet;
		glm::ivec2 pos;
		int sigma;

		for (pos.x = 0, pos.y = size.y, sigma = 2 * r.y + r.x * (1 - 2 * size.y); r.y * pos.x <= r.x * pos.y; pos.x++)
		{
			coords.emplace_back(pos);
			if (sigma >= 0)
			{
				sigma += fr.x * (1 - pos.y);
				pos.y--;
			}
			sigma += r.y * ((4 * pos.x) + 6);
		}
		for (pos.x = size.x, pos.y = 0, sigma = 2 * r.x + r.y * (1 - 2 * size.x); r.x * pos.y <= r.y * pos.x; pos.y++)
		{
			secondSet.emplace_back(pos);
			if (sigma >= 0)
			{
				sigma += fr.y * (1 - pos.x);
				pos.x--;
			}
			sigma += r.x * ((4 * pos.y) + 6);
		}
		for (int i = secondSet.size() - 1; i >= 0; --i)
			coords.emplace_back(secondSet[i]);
		return coords;
	}
	glm::ivec3 mapVector(const glm::ivec3& vector, const glm::ivec3& dim)
	{
		glm::ivec3 v;
		v[dim.x] = vector.x;
		v[dim.y] = vector.y;
		v[dim.z] = vector.z;
		return v;
	}
}

void vox::editor::shape::ShapeCylinder::mesh(std::vector<glm::vec3>& vertices, std::vector<unsigned int>& indices) const
{
	const auto start = hen::math::min(getStart(), getEnd());
	const auto end = hen::math::max(getStart(), getEnd());
	
	const auto dim = m_axis == hen::math::Axis::Z ? glm::ivec3{ 0, 1, 2 } : m_axis == hen::math::Axis::Y ? glm::ivec3{ 2, 0, 1 } : glm::ivec3{ 1, 2, 0 };
	const auto size = end - start;
	auto center = (start + end) / 2;
	center[dim.z] = start[dim.z];

	// What we don't do to deal with nonsensical rounding issues with integer math...
	if (start[dim.x] < 0 && start[dim.x] != -end[dim.x] && start[dim.x] % 2 == -1)
		center[dim.x]--;
	if (start[dim.y] < 0 && start[dim.y] != -end[dim.y] && start[dim.y] % 2 == -1)
		center[dim.y]--;

	// Special case; if the size is either one or two blocks wide/deep, simplify to a rectangle
	if (size[dim.x] <= 1 || size[dim.y] <= 1)
	{
		meshBox(getStart(), getEnd(), vertices, indices);
		return;
	}

	// Handle regular cylinder
	auto oldPos = glm::ivec2{ 0, size[dim.y] / 2 };
	std::vector<glm::ivec2> steps;

	for (const auto pos : getQuarterCylinderCoordinates(glm::ivec2{ size[dim.x] / 2, size[dim.y] / 2 }))
	{
		//const glm::ivec2 mod{ pos.x >= 0 ? size[dim.x] % 2 : 0, pos.y >= 0 ? size[dim.y] % 2 : 0 };
		//meshPart(center + glm::ivec3{ 0, 0, 1 }, pos, mod, vertices, indices);

		const auto delta = pos - oldPos;
		if (std::abs(delta.x) + std::abs(delta.y) != 1)
		{
			steps.emplace_back(pos);
			steps.emplace_back(pos);
		}
		else
			steps.back() += delta;
		oldPos = pos;
	}

	for (unsigned int i = 0; i + 1 < steps.size(); i += 2)
	{
		const auto min = hen::math::min(steps[i], steps[i + 1]);
		const auto max = hen::math::max(steps[i], steps[i + 1]) + 1;
		const glm::ivec2 minMod{ size[dim.x] % 2, size[dim.y] % 2 };
		const glm::ivec2 maxMod{ size[dim.x] % 2, size[dim.y] % 2 };
		//meshBox(center + min + minMod, center + max + maxMod - 1, vertices, indices);


		const auto index = vertices.size();
		const glm::ivec2 height{ 0, size[dim.z] + 1 };
		for (unsigned int j = 0; j < 2; ++j)
		{
			vertices.emplace_back(center + mapVector(glm::ivec3{ max.x + maxMod.x, min.y + minMod.y, height[j] }, dim));
			vertices.emplace_back(center + mapVector(glm::ivec3{ max.x + maxMod.x, max.y + maxMod.y, height[j] }, dim));
			vertices.emplace_back(center + mapVector(glm::ivec3{ min.x + minMod.x, max.y + maxMod.y, height[j] }, dim));
			vertices.emplace_back(center + mapVector(glm::ivec3{ 1 - max.x, min.y + minMod.y, height[j] }, dim));
			vertices.emplace_back(center + mapVector(glm::ivec3{ 1 - max.x, max.y + maxMod.y, height[j] }, dim));
			vertices.emplace_back(center + mapVector(glm::ivec3{ 1 - min.x, max.y + maxMod.y, height[j] }, dim));
			vertices.emplace_back(center + mapVector(glm::ivec3{ max.x + maxMod.x, 1 - min.y, height[j] }, dim));
			vertices.emplace_back(center + mapVector(glm::ivec3{ max.x + maxMod.x, 1 - max.y, height[j] }, dim));
			vertices.emplace_back(center + mapVector(glm::ivec3{ min.x + minMod.x, 1 - max.y, height[j] }, dim));
			vertices.emplace_back(center + mapVector(glm::ivec3{ 1 - max.x, 1 - min.y, height[j] }, dim));
			vertices.emplace_back(center + mapVector(glm::ivec3{ 1 - max.x, 1 - max.y, height[j] }, dim));
			vertices.emplace_back(center + mapVector(glm::ivec3{ 1 - min.x, 1 - max.y, height[j] }, dim));
		}

		for (unsigned int j = 0; j < 4; j++)
		{
			indices.emplace_back(index + 3 * j + 0); indices.emplace_back(index + 3 * j + 1);
			indices.emplace_back(index + 3 * j + 1); indices.emplace_back(index + 3 * j + 2);
			indices.emplace_back(index + 3 * j + 12); indices.emplace_back(index + 3 * j + 13);
			indices.emplace_back(index + 3 * j + 13); indices.emplace_back(index + 3 * j + 14);

			if (i < steps.size() - 2) { indices.emplace_back(index + 3 * j + 0); indices.emplace_back(index + 3 * j + 12); }
			indices.emplace_back(index + 3 * j + 1); indices.emplace_back(index + 3 * j + 13);
		}
	}
}