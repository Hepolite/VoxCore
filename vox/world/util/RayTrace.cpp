
#include "vox/world/util/RayTrace.h"

#include "vox/world/World.h"

vox::world::RayTrace::RayTrace(const World* world, const glm::ivec3& start, const glm::ivec3& end)
	: RayTrace(world, glm::vec3{ start } + 0.5f, glm::vec3{ end - start }, glm::length(glm::vec3{ end - start }))
{}
vox::world::RayTrace::RayTrace(const World* world, const glm::vec3& start, const glm::vec3& end)
	: RayTrace(world, start, end - start, glm::length(end - start))
{}
vox::world::RayTrace::RayTrace(const World* world, const glm::vec3& start, const glm::vec3& direction, float length) :
	m_world(world), m_position(start)
{
	m_direction = length * glm::normalize(direction);
	m_step = hen::math::sign(m_direction);

	m_tMax = hen::math::intbound(start, m_direction);
	m_tDelta = m_step / m_direction;
}

unsigned int vox::world::RayTrace::getBlock() const
{
	return m_world == nullptr ? 0 : m_world->getBlock(getBlockPosition());
}
void vox::world::RayTrace::next()
{
	m_oldPosition = m_position;
	if (m_tMax.x < m_tMax.y && m_tMax.x < m_tMax.z)
	{
		m_position.x += m_step.x;
		m_tMax.x += m_tDelta.x;
	}
	else if (m_tMax.y < m_tMax.z)
	{
		m_position.y += m_step.y;
		m_tMax.y += m_tDelta.y;
	}
	else
	{
		m_position.z += m_step.z;
		m_tMax.z += m_tDelta.z;
	}
}
bool vox::world::RayTrace::isValid() const
{
	return m_tMax.x < 1.0f || m_tMax.y < 1.0f || m_tMax.z < 1.0f;
}
