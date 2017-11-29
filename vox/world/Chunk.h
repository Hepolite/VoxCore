
#pragma once

#include "vox/world/data/BlockRegion.h"
#include "vox/world/data/BlockQuery.h"
#include "vox/world/data/ChunkData.h"
#include "vox/world/Side.h"

#include <glm/vec3.hpp>

#include <memory>

namespace vox
{
	namespace world
	{
		class Chunk
		{
		public:
			Chunk(const glm::ivec3& pos) : m_pos(pos) {}
			Chunk(const Chunk&) = delete;
			Chunk(Chunk&&) = default;
			~Chunk() = default;

			Chunk& operator=(const Chunk&) = delete;
			Chunk& operator=(Chunk&&) = default;

			inline glm::ivec3 getPos() const { return m_pos; }

			void setNeighbor(Chunk* neighbor, const Side& side);
			void setNearest(Chunk* above, Chunk* below);
			inline Chunk* getNeighbor(const Side& side) const { return m_neighbors[side.id]; }
			inline Chunk* getNearestAbove() const { return m_nearestAbove; }
			inline Chunk* getNearestBelow() const { return m_nearestBelow; }

			data::BlockData getBlock(const glm::uvec3& pos) const;
			void acceptReadQuery(data::BlockQuery& query) const;
			void acceptWriteQuery(data::BlockQuery& query);

			data::ChunkDataFlat& getActualData();
			data::BlockRegion getMeshingData() const;
			data::ChunkDataRLE getStoringData() const;
			void injectChunkData(data::ChunkDataRLE&& data);

			void expand();
			void contract();

			inline unsigned int memusage() const { return m_dataFlat.memusage() + m_dataRLE.memusage(); }
			inline bool empty() const { return m_dataFlat.empty() && m_dataRLE.empty(); }

		private:
			void getMeshingData(data::BlockRegion& region, const glm::uvec3& position, const glm::uvec3& offset, const glm::uvec3& size) const;

			glm::ivec3 m_pos;

			Chunk* m_neighbors[Side::COUNT] = { nullptr };
			Chunk* m_nearestAbove = nullptr;
			Chunk* m_nearestBelow = nullptr;

			data::ChunkDataFlat m_dataFlat;
			data::ChunkDataRLE m_dataRLE;
		};
	}
}