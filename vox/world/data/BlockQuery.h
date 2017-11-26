
#pragma once

#include "vox/world/data/BlockData.h"

#include <glm/vec3.hpp>

#include <vector>

namespace vox
{
	namespace data
	{
		using Query = std::pair<BlockData, unsigned short>;
		using QueryList = std::vector<Query>;

		class BlockQuery
		{
		public:
			BlockQuery() = default;
			BlockQuery(const BlockQuery&) = delete;
			BlockQuery(BlockQuery&&) = default;
			virtual ~BlockQuery() = default;

			BlockQuery& operator=(const BlockQuery&) = delete;
			BlockQuery& operator=(BlockQuery&&) = default;

			inline unsigned int memusage() const { return size() * sizeof(Query); }
			inline unsigned int size() const { return m_nodes.size(); }
			inline bool empty() const { return m_nodes.empty(); }

			inline BlockData front() const { return empty() ? BlockData{} : m_nodes.front().first; }
			inline BlockData back() const { return empty() ? BlockData{} : m_nodes.back().first; }
			inline auto begin() { return m_nodes.begin(); }
			inline auto begin() const { return m_nodes.begin(); }
			inline auto end() { return m_nodes.end(); }
			inline auto end() const { return m_nodes.end(); }

			void add(const BlockData& data, const glm::uvec3& pos);
			void add(const BlockData& data, const glm::uvec3& start, const glm::uvec3& end);
			bool has(const glm::uvec3& pos) const;
			BlockData get(const glm::uvec3& pos) const;

			inline glm::uvec3 min() const { return m_min; }
			inline glm::uvec3 max() const { return m_max; }

		private:
			void limit(const glm::uvec3& lower, const glm::uvec3& upper);
			void set(const BlockData& data, const glm::uvec3& pos);
			QueryList::iterator getLocation(const glm::uvec3& pos, const BlockData& data);
			QueryList::const_iterator getLocation(const glm::uvec3& pos) const;

			QueryList m_nodes;
			glm::uvec3 m_min;
			glm::uvec3 m_max;
		};
	}
}