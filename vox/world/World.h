
#pragma once

#include "vox/world/Chunk.h"

#include "hen/io/File.h"
#include "hen/util/MathEnum.h"

#include <glm/vec3.hpp>
#include <glm/gtx/hash.hpp>

#include <string>
#include <memory>
#include <unordered_map>

namespace vox
{
	namespace world
	{
		class World
		{
		public:
			World(const std::string& name) : m_name(name) {}
			World(const World&) = delete;
			World(World&&) = delete;
			~World() = default;

			World& operator=(const World&) = delete;
			World& operator=(World&&) = delete;

			///////////////////////////////////////////////////////

			void onProcess(float dt);
			void onLoad(const hen::io::File& file);
			void onUnload();

			inline const std::string& getName() const { return m_name; }

			///////////////////////////////////////////////////////

			Chunk* getChunk(const glm::ivec3& cpos) const;
			std::vector<Chunk*> getChunks() const;

			unsigned int getBlock(const glm::ivec3& pos) const;
			void setBlock(unsigned int id, const glm::ivec3& pos);
			void setBlockCylinder(unsigned int id, const glm::ivec3& start, const glm::ivec3& end, hen::math::Axis axis);
			void setBlockEllipse(unsigned int id, const glm::ivec3& start, const glm::ivec3& end);
			void setBlockLine(unsigned int id, const glm::ivec3& start, const glm::ivec3& end);
			void setBlockRectangle(unsigned int id, const glm::ivec3& start, const glm::ivec3& end);

		private:
			Chunk* createChunk(const glm::ivec3& cpos);
			Chunk* getOrCreateChunk(const glm::ivec3& cpos);
			void deleteChunk(const glm::ivec3& cpos);

			const std::string m_name;

			std::unordered_map<glm::ivec3, std::unique_ptr<Chunk>> m_chunks;
		};
	}
}