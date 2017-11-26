
#pragma once

#include "vox/world/data/BlockRegion.h"
#include "vox/world/data/BlockQuery.h"
#include "vox/world/data/ChunkData.h"
#include "vox/world/Side.h"

#include <memory>

namespace vox
{
	namespace world
	{
		class Chunk
		{
		public:
			void setNeighbor(Chunk* neighbor, const Side& side);
			inline Chunk* getNeighbor(const Side& side) const { return m_neighbors[side.id]; }

			data::BlockData getBlock(const glm::uvec3& pos) const;
			void acceptReadQuery(data::BlockQuery& query) const;
			void acceptWriteQuery(data::BlockQuery& query);

			data::BlockRegion getMeshingData() const;
			data::ChunkDataRLE getStoringData() const;
			void injectChunkData(data::ChunkDataRLE&& data);

			unsigned int memusage() const;
			bool empty() const;

		private:
			void getMeshingData(data::BlockRegion& region, const glm::uvec3& position, const glm::uvec3& offset, const glm::uvec3& size) const;

			Chunk* m_neighbors[Side::COUNT] = { nullptr };

			data::ChunkDataFlat m_dataFlat;
			data::ChunkDataRLE m_dataRLE;
		};
	}
}