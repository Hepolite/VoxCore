
#pragma once

#include "vox/world/data/BlockData.h"

#include <glm/vec3.hpp>

#include <vector>

namespace vox
{
	namespace data
	{
		class BlockQuery
		{
			using Query = std::pair<BlockData, unsigned int>;
			using QueryList = std::vector<Query>;

		public:
			BlockQuery() : BlockQuery(true, true, true) {}
			BlockQuery(bool useIds, bool useColors, bool useAlpha)
				: m_bitmask((useIds * BlockData::BITMASK_ID) | (useColors * BlockData::BITMASK_COLOR) | (useAlpha * BlockData::BITMASK_ALPHA))
			{}
			BlockQuery(const BlockQuery&) = delete;
			BlockQuery(BlockQuery&&) = default;
			virtual ~BlockQuery() = default;

			BlockQuery& operator=(const BlockQuery&) = delete;
			BlockQuery& operator=(BlockQuery&&) = default;

			inline unsigned int memusage() const { return size() * sizeof(BlockQuery); }
			inline unsigned int size() const { return m_nodes.size(); }
			inline bool empty() const { return m_nodes.empty(); }

			inline BlockData front() const { return empty() ? BlockData{} : m_nodes.front().first; }
			inline BlockData back() const { return empty() ? BlockData{} : m_nodes.back().first; }
			inline QueryList::iterator begin() { return m_nodes.begin(); }
			inline QueryList::iterator end() { return m_nodes.end(); }

			void add(const BlockData& data, const glm::uvec3& pos);
			void add(const BlockData& data, const glm::uvec3& start, const glm::uvec3& end);
			bool get(BlockData& data, const glm::uvec3& pos) const;

			inline unsigned int bitmask() const { return m_bitmask; }
			inline glm::uvec3 min() const { return m_min; }
			inline glm::uvec3 max() const { return m_max; }

		private:
			unsigned int getIndex(const glm::uvec3& pos) const;
			void limit(const glm::uvec3& lower, const glm::uvec3& upper);
			void set(const BlockData& data, const glm::uvec3& pos);

			QueryList m_nodes;
			unsigned int m_bitmask;
			glm::uvec3 m_min;
			glm::uvec3 m_max;
		};
	}
}