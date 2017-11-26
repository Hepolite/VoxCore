
#pragma once

#include "vox/world/data/BlockData.h"
#include "vox/world/data/BlockRegion.h"
#include "vox/world/data/BlockQuery.h"

#include <queue>

namespace vox
{
	namespace data
	{
		class ChunkDataFlat
		{
			friend class ChunkDataTranslator;

		public:
			void acceptRegionQuery(BlockRegion& region, const glm::uvec3& dataOffset, const glm::ivec3& regionOffset, const glm::uvec3& size) const;
			void acceptReadQuery(BlockQuery& query) const;
			void acceptWriteQuery(BlockQuery& query);
			void setBlock(const glm::uvec3& pos, const BlockData& data);
			void setBlock(unsigned int index, const BlockData& data);
			BlockData getBlock(const glm::uvec3& pos) const;
			BlockData getBlock(unsigned int index) const;

			unsigned int memusage() const;
			unsigned int size() const;
			bool empty() const;
			void expand();
			void forget();

		private:
			BlockDataList m_data;

			std::queue<Query> m_lightPropagationNodes;
			std::queue<Query> m_lightRemovalNodes;
		};

		class ChunkDataRLE
		{
			friend class ChunkDataTranslator;

		public:
			using Node = std::pair<BlockData, unsigned int>;
			using NodeList = std::vector<Node>;

			void acceptRegionQuery(BlockRegion& region, const glm::uvec3& dataOffset, const glm::ivec3& regionOffset, const glm::uvec3& size) const;
			void acceptReadQuery(BlockQuery& query) const;
			BlockData getBlock(const glm::uvec3& pos) const;
			BlockData getBlock(unsigned int index) const;

			unsigned int memusage() const;
			unsigned int size() const;
			bool empty() const;
			void forget();

			inline NodeList& getRawData() { return m_data; }
			inline const NodeList& getRawData() const { return m_data; }

		private:
			NodeList m_data;
		};
	}
}