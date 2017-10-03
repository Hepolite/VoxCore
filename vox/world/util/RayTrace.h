
#pragma once

#include "vox/world/World.h"

#include "hen/util/MathLib.h"

namespace vox
{
	namespace world
	{
		class RayTrace
		{
		public:
			RayTrace(const World* world, const glm::ivec3& start, const glm::ivec3& end);
			RayTrace(const World* world, const glm::vec3& start, const glm::vec3& end);
			RayTrace(const World* world, const glm::vec3& start, const glm::vec3& direction, float length);
			~RayTrace() = default;

			bool isValid() const;
			
			inline unsigned int nextBlock() { next(); return getBlock(); }
			inline glm::vec3 nextPosition() { next(); return getPosition(); }
			inline glm::ivec3 nextBlockPosition() { next(); return getBlockPosition(); }

			inline const World* getWorld() const { return m_world; }
			inline glm::vec3 getPosition() const { return m_position; }
			inline glm::vec3 getOldPosition() const { return m_oldPosition; }
			inline glm::ivec3 getBlockPosition() const { return hen::math::floor(m_position); }
			inline glm::ivec3 getOldBlockPosition() const { return hen::math::floor(m_oldPosition); }
			unsigned int getBlock() const;

		private:
			void next();

			const World* m_world;
			glm::vec3 m_position, m_oldPosition;
			glm::vec3 m_direction;
			glm::vec3 m_step;

			glm::vec3 m_tMax;
			glm::vec3 m_tDelta;
		};
	}
}