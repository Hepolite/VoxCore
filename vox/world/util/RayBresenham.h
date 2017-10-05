
#pragma once

#include "vox/world/World.h"

#include <glm/vec3.hpp>

namespace vox
{
	namespace world
	{
		class RayBresenham
		{
		public:
			RayBresenham(const World* world, const glm::ivec3& start, const glm::ivec3& end);
			~RayBresenham() = default;

			inline bool isValid() const { return m_currentStep <= m_totalSteps; }

			inline unsigned int nextBlock() { next(); return getBlock(); }
			inline glm::ivec3 nextBlockPosition() { next(); return getBlockPosition(); }

			inline const World* getWorld() const { return m_world; }
			inline glm::ivec3 getBlockPosition() const { return m_position; }
			inline glm::ivec3 getOldBlockPosition() const { return m_oldPosition; }
			unsigned int getBlock() const;

		private:
			void next();

			const World* m_world;
			glm::ivec3 m_position, m_oldPosition;
			glm::ivec3 m_delta;
			glm::ivec3 m_step;
			glm::ivec3 m_error;

			int m_totalSteps;
			int m_currentStep = 0;
		};
	}
}