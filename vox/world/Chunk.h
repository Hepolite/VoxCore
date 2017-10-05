
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
			void acceptQuery(data::BlockReadQuery& query) const;
			void acceptQuery(data::BlockWriteQuery& query);

			data::BlockRegion getMeshingData() const;

			bool isEmpty() const;

		private:
			Chunk* m_neighbors[Side::COUNT] = { nullptr };

			data::ChunkDataFlat m_dataFlat;
			data::ChunkDataRLE m_dataRLE;
			data::ChunkData* m_data = nullptr;
		};
	}
}