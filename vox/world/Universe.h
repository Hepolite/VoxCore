
#pragma once

#include "vox/world/BlockRegistry.h"
#include "vox/world/World.h"

#include <memory>
#include <string>
#include <vector>

namespace vox
{
	namespace world
	{
		class Universe
		{
		public:
			Universe() = default;
			Universe(const Universe&) = delete;
			Universe(Universe&&) = delete;
			~Universe() = default;

			Universe& operator=(const Universe&) = delete;
			Universe& operator=(Universe&&) = delete;

			void onProcess(float dt);
			void onLoad();

			inline const BlockRegistry& getRegistry() const { return m_registry; }

			World* createWorld(const std::string& name);
			void destroyWorld(const std::string& name);
			World* getWorld(const std::string& name) const;
			std::vector<World*> getWorlds() const;

		private:
			void loadRegistry();
			void loadBlocks();
			void loadBlock(Block& block);

			BlockRegistry m_registry;

			std::unordered_map<std::string, std::unique_ptr<World>> m_worlds;
		};
	}
}