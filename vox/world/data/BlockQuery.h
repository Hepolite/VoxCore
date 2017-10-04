
#pragma once

#include "vox/world/data/BlockData.h"
#include "vox/world/data/BlockRegion.h"

#include <glm/vec3.hpp>

#include <vector>

namespace vox
{
	namespace data
	{
		struct BlockQuery
		{
			BlockQuery() {}
			BlockQuery(const BlockData& data, const glm::ivec3& pos) : m_data(data), m_pos(pos) {}

			BlockData m_data;
			const glm::ivec3 m_pos;
		};

		struct BlockQueryList
		{
			using QueryList = std::vector<BlockQuery>;

			BlockQueryList() {}
			BlockQueryList(const QueryList& data) : m_data(data) {}
			BlockQueryList(QueryList&& data) : m_data(std::move(data)) {}

			QueryList m_data;
		};

		struct BlockQueryRectangle
		{
			BlockQueryRectangle() {}
			BlockQueryRectangle(const BlockRegion& data) : m_data(data) {}
			BlockQueryRectangle(const BlockData& data, const glm::ivec3& start, const glm::ivec3& end) : m_data({ start, end - start + 1, data }) {}

			BlockRegion m_data;
		};
	}
}