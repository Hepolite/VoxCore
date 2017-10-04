
#pragma once

#include "vox/world/World.h"

#include <glm/gtx/hash.hpp>
#include <glm/vec3.hpp>

namespace vox
{
	namespace world
	{
		class Location
		{
		public:
			Location() = default;
			Location(const World* world, const glm::ivec3& pos) : m_world(world), m_pos(pos) {}
			Location(const Location&) = default;
			Location(Location&&) = default;
			~Location() = default;

			Location& operator=(const Location& other) = default;
			Location& operator=(Location&& other) = default;
			bool operator==(const Location& l) const { return m_world == l.m_world && m_pos == l.m_pos; }
			bool operator==(Location&& l) const { return m_world == l.m_world && m_pos == l.m_pos; }
			
			inline const World* getWorld() const { return m_world; }
			inline glm::ivec3 getPos() const { return m_pos; }
			inline void setWorld(World* world) { m_world = world; }
			inline void setPos(const glm::ivec3& pos) { m_pos = pos; }

		private:
			const World* m_world = nullptr;
			glm::ivec3 m_pos;
		};
	}
}

namespace std
{
	template<>
	struct hash<vox::world::Location>
	{
		size_t operator()(const vox::world::Location& location) const
		{
			size_t seed = 0;
			glm::detail::hash_combine(seed, hash<const vox::world::World*>{}(location.getWorld()));
			glm::detail::hash_combine(seed, hash<glm::ivec3>{}(location.getPos()));
			return seed;
		}
	};
}