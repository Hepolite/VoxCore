
#pragma once

#include "vox/world/data/BlockRegion.h"
#include "vox/world/Location.h"
#include "vox/world/render/ChunkModelVertex.h"

#include <memory>
#include <vector>

namespace vox
{
	namespace world
	{
		namespace render
		{
			class ChunkMeshTask
			{
			public:
				ChunkMeshTask() = default;
				ChunkMeshTask(const glm::ivec3& pos, data::BlockRegion&& data) : m_pos(pos), m_data(std::move(data)) {}
				ChunkMeshTask(const ChunkMeshTask&) = delete;
				ChunkMeshTask(ChunkMeshTask&&) = default;
				~ChunkMeshTask() = default;

				ChunkMeshTask& operator=(const ChunkMeshTask&) = delete;
				ChunkMeshTask& operator=(ChunkMeshTask&& other) = default;

				inline glm::ivec3 getPos() const { return m_pos; }
				inline const data::BlockRegion& getData() const { return m_data; }
				inline std::vector<unsigned int>& getIndices() { return m_indices; }
				inline std::vector<ChunkModelVertex>& getVertices() { return m_vertices; }

			private:
				glm::ivec3 m_pos;
				data::BlockRegion m_data;
				std::vector<unsigned int> m_indices;
				std::vector<ChunkModelVertex> m_vertices;
			};
		}
	}
}