
#pragma once

#include "vox/world/data/ChunkQuery.h"

#include <glm/vec3.hpp>

#include <vector>

namespace vox
{
	namespace tool
	{
		class WorldFilterKernel
		{
		public:
			using Node = std::pair<glm::ivec3, int>;
			using NodeList = std::vector<Node>;

			WorldFilterKernel() = default;
			WorldFilterKernel(const NodeList& nodes) : m_nodes(nodes) {}

			void add(const glm::ivec3& pos, int weight);
			void apply(const data::ChunkQuery& source, data::ChunkQuery& target, const glm::ivec3& pos) const;

		private:
			NodeList m_nodes;
		};

		data::ChunkQuery generic(const data::ChunkQuery& query, const std::vector<WorldFilterKernel>& kernels);

		data::ChunkQuery erode(const data::ChunkQuery& query);
		data::ChunkQuery smooth(const data::ChunkQuery& query);
	}
}