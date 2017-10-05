
#pragma once

#include <glm/vec3.hpp>

#include <optional>
#include <vector>

namespace vox
{
	namespace io
	{
		class RegionHeader
		{
			using RegionNode = std::pair<unsigned int, unsigned int>;
			using RegionNodeList = std::vector<RegionNode>;

		public:
			RegionHeader();

			void update(const glm::uvec3& chunk, unsigned int size);
			void remove(const glm::uvec3& chunk);

			std::optional<unsigned int> getDataSize(const glm::uvec3& chunk) const;
			std::optional<unsigned int> getDataLocation(const glm::uvec3& chunk) const;

		private:
			unsigned int getIndex(const glm::uvec3& chunk) const;

			RegionNodeList m_nodes;
		};
	}
}