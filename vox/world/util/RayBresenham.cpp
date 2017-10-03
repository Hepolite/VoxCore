
#include "vox/world/util/RayBresenham.h"

#include "hen/util/MathLib.h"

vox::world::RayBresenham::RayBresenham(const World* world, const glm::ivec3& start, const glm::ivec3& end)
	: m_world(world), m_position(start)
{
	m_delta = end - start;
	m_step = hen::math::sign(m_delta);

	m_delta = hen::math::abs(m_delta);
	m_totalSteps = (m_delta.x > m_delta.y && m_delta.x > m_delta.z) ? m_delta.x : (m_delta.y > m_delta.z) ? m_delta.y : m_delta.z;
}

unsigned int vox::world::RayBresenham::getBlock() const
{
	return m_world == nullptr ? 0 : m_world->getBlock(getBlockPosition());
}
void vox::world::RayBresenham::next()
{
	m_oldPosition = m_position;

	m_currentStep++;
	m_error += m_delta;
	for (unsigned int i = 0; i < 3; ++i)
	{
		if (2 * m_error[i] >= m_totalSteps)
		{
			m_position[i] += m_step[i];
			m_error[i] -= m_totalSteps;
		}
	}
}

