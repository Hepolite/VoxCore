
#pragma once

#include "vox/world/data/ChunkCompositeData.h"
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

			data::BlockQuery getBlock(const glm::ivec3& pos) const;
			data::BlockQueryList getBlocks(const std::vector<glm::ivec3>& locations) const;
			data::BlockQueryRectangle getBlocks(const glm::ivec3& start, const glm::ivec3& end) const;
			void setBlock(const data::BlockQuery& query);
			void setBlocks(const data::BlockQueryList& query);
			void setBlocks(const data::BlockQueryRectangle& query);

			data::BlockRegion getMeshingData() const;
			data::ChunkDataRLE getCompressedData() const;

			bool isEmpty() const;

		private:
			Chunk* m_neighbors[Side::COUNT] = { nullptr };

			data::ChunkCompositeData m_data;
		};
	}
}