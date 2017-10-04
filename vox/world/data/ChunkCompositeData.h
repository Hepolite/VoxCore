
#pragma once

#include "vox/world/data/BlockQuery.h"
#include "vox/world/data/ChunkDataFlat.h"
#include "vox/world/data/ChunkDataRLE.h"
#include "vox/world/data/ChunkDataUniform.h"

#include <glm/vec3.hpp>

namespace vox
{
	namespace data
	{
		class ChunkCompositeData
		{
		public:
			enum class DataFormat { NONE, FLAT, RLE, UNIFORM };

			void optimizeDataFormat();
			void setDataFormat(DataFormat format);
			inline DataFormat getDataFormat() const { return m_format; }

			BlockQuery requestBlock(const glm::ivec3& pos) const;
			BlockQueryList requestBlocks(const std::vector<glm::ivec3>& locations) const;
			BlockQueryRectangle requestBlocks(const glm::ivec3& start, const glm::ivec3& end) const;
			void reguestBlocks(BlockRegion& region, const glm::ivec3& start, const glm::ivec3& end, const glm::ivec3& offset) const;

			void assignChange(const BlockQuery& query);
			void assignChange(const BlockQueryList& query);
			void assignChange(const BlockQueryRectangle& query);

		private:
			DataFormat m_format = DataFormat::NONE;

			data::ChunkDataFlat m_dataFlat;
			data::ChunkDataRLE m_dataRLE;
			data::ChunkDataUniform m_dataUniform;
		};
	}
}