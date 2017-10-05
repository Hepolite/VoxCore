
#pragma once

#include "vox/world/data/BlockData.h"

#include <glm/vec3.hpp>

#include <vector>

namespace vox
{
	namespace data
	{
		using BlockQuery = std::pair<BlockData, unsigned int>;
		using BlockQueryList = std::vector<BlockQuery>;

		class BlockBaseQuery
		{
		public:
			BlockBaseQuery() : BlockBaseQuery(true, true, true) {}
			BlockBaseQuery(bool writeIds, bool writeColors, bool writeAlpha)
				: m_bitmask((writeIds * BlockData::BITMASK_ID) | (writeColors * BlockData::BITMASK_COLOR) | (writeAlpha * BlockData::BITMASK_ALPHA))
			{}
			BlockBaseQuery(const BlockBaseQuery&) = delete;
			BlockBaseQuery(BlockBaseQuery&&) = default;
			virtual ~BlockBaseQuery() = default;

			BlockBaseQuery& operator=(const BlockBaseQuery&) = delete;
			BlockBaseQuery& operator=(BlockBaseQuery&&) = default;

			inline unsigned int memusage() const { return size() * sizeof(BlockQuery); }
			inline unsigned int size() const { return m_nodes.size(); }
			inline bool empty() const { return m_nodes.empty(); }

			inline BlockData front() const { return empty() ? 0 : m_nodes.front().first; }
			inline BlockData back() const { return empty() ? 0 : m_nodes.back().first; }
			inline BlockQueryList::iterator begin() { return m_nodes.begin(); }
			inline BlockQueryList::iterator end() { return m_nodes.end(); }

			inline glm::uvec3 min() const { return m_min; }
			inline glm::uvec3 max() const { return m_max; }
			inline unsigned int bitmask() const { return m_bitmask; }

		protected:
			unsigned int getIndex(const glm::uvec3& pos) const;
			void limit(const glm::uvec3& lower, const glm::uvec3& upper);

			BlockQueryList m_nodes;
			unsigned int m_bitmask;
			glm::uvec3 m_min;
			glm::uvec3 m_max;
		};

		class BlockReadQuery : public BlockBaseQuery
		{
		public:
			BlockReadQuery() = default;
			BlockReadQuery(bool readIds, bool readColors, bool readAlpha) : BlockBaseQuery(readIds, readColors, readAlpha) {}

			void add(const glm::uvec3& pos);
			void add(const glm::uvec3& start, const glm::uvec3& end);
		};

		class BlockWriteQuery : public BlockBaseQuery
		{
		public:
			BlockWriteQuery() = default;
			BlockWriteQuery(bool writeIds, bool writeColors, bool writeAlpha) : BlockBaseQuery(writeIds, writeColors, writeAlpha) {}

			void add(const BlockData& data, const glm::uvec3& pos);
			void add(const BlockData& data, const glm::uvec3& start, const glm::uvec3& end);
		};
	}
}