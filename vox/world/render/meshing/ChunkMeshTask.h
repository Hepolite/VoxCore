
#pragma once

#include "vox/chunk/BlockVolume.h"
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
				ChunkMeshTask(const Location& location, std::unique_ptr<chunk::BlockVolume>&& data) : m_location(location), m_data(std::move(data)) {}
				ChunkMeshTask(const ChunkMeshTask&) = delete;
				ChunkMeshTask(ChunkMeshTask&&) = default;
				~ChunkMeshTask() = default;

				ChunkMeshTask& operator=(const ChunkMeshTask&) = delete;
				ChunkMeshTask& operator=(ChunkMeshTask&& other) = default;

				inline Location getLocation() const { return m_location; }
				inline const chunk::BlockVolume& getData() const { return *m_data; }
				inline std::vector<unsigned int>& getIndices() { return m_indices; }
				inline std::vector<ChunkModelVertex>& getVertices() { return m_vertices; }

			private:
				Location m_location;
				std::unique_ptr<chunk::BlockVolume> m_data;
				std::vector<unsigned int> m_indices;
				std::vector<ChunkModelVertex> m_vertices;
			};
		}
	}
}