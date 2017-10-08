
#pragma once

namespace vox
{
	namespace world
	{
		class World;
	}

	namespace events
	{
		class WorldCreate
		{
		public:
			WorldCreate(world::World* world) : m_world(world) {}
			~WorldCreate() = default;

			inline world::World* getWorld() const { return m_world; }

		private:
			world::World* m_world;
		};

		class WorldDestroy
		{
		public:
			WorldDestroy(world::World* world) : m_world(world) {}
			~WorldDestroy() = default;

			inline world::World* getWorld() const { return m_world; }

		private:
			world::World* m_world;
		};
	}
}