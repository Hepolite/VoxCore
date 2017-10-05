
#pragma once

#include "vox/world/data/BlockData.h"
#include "vox/world/data/BlockRegion.h"
#include "vox/world/data/BlockQuery.h"

namespace vox
{
	namespace data
	{
		class ChunkData
		{
		public:
			virtual ~ChunkData() = default;

			virtual BlockData getBlock(const glm::uvec3& pos) const = 0;
			virtual void fillRegion(BlockRegion& region, const glm::uvec3& dataOffset, const glm::ivec3& regionOffset, const glm::uvec3& size) const = 0;
			virtual void acceptQuery(BlockReadQuery& query) const = 0;
			virtual void acceptQuery(BlockWriteQuery& query) = 0;

		protected:
			unsigned int getIndex(const glm::uvec3& pos) const;
		};

		class ChunkDataFlat : public ChunkData
		{
			friend class ChunkDataTranslator;

		public:
			virtual BlockData getBlock(const glm::uvec3& pos) const override final;
			virtual void fillRegion(BlockRegion& region, const glm::uvec3& dataOffset, const glm::ivec3& regionOffset, const glm::uvec3& size) const override final;
			virtual void acceptQuery(BlockReadQuery& query) const override final;
			virtual void acceptQuery(BlockWriteQuery& query) override final;

			bool empty() const;
			void expand();
			void forget();

		private:
			BlockData getBlock(unsigned int index) const;
			void swapBlock(BlockData& data, unsigned int index);

			BlockDataList m_data;
		};

		class ChunkDataRLE : public ChunkData
		{
			friend class ChunkDataTranslator;

			using Node = std::pair<BlockData, unsigned int>;
			using NodeList = std::vector<Node>;

		public:
			virtual BlockData getBlock(const glm::uvec3& pos) const override final;
			virtual void fillRegion(BlockRegion& region, const glm::uvec3& dataOffset, const glm::ivec3& regionOffset, const glm::uvec3& size) const override final;
			virtual void acceptQuery(BlockReadQuery& query) const override final;
			virtual void acceptQuery(BlockWriteQuery& query) override final;

			bool empty() const;
			void forget();

		private:
			BlockData getBlock(unsigned int index) const;

			NodeList m_data;
		};
	}
}