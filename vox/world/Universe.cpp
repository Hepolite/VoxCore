
#include "vox/world/Universe.h"

#include "vox/events/World.h"

#include "hen/core/Core.h"
#include "hen/event/EventBus.h"
#include "hen/io/File.h"
#include "hen/io/Folder.h"
#include "hen/io/XMLFile.h"
#include "hen/util/StringLib.h"

#include <Log.h>

#include <memory>
#include <unordered_map>

void vox::world::Universe::onProcess(float dt)
{
}

vox::world::World* vox::world::Universe::createWorld(const std::string& name)
{
	if (getWorld(name) != nullptr)
	{
		LOG_WARNING << "Attempted to create a world with name " << name << ", which already exists";
		return nullptr;
	}
	auto world = std::make_unique<World>(name);
	auto ptr = world.get();
	m_worlds[name] = std::move(world);
	hen::Core::getEventBus().post(events::WorldCreate{ ptr });
	LOG_INFO << "Created world " << name;
	return ptr;
}
void vox::world::Universe::destroyWorld(const std::string& name)
{
	auto world = getWorld(name);
	if (world == nullptr)
	{
		LOG_WARNING << "Attempted to destroy a world with name " << name << ", which does not exist";
		return;
	}
	
	hen::Core::getEventBus().post(events::WorldDestroy{ world });
	m_worlds.erase(name);
	LOG_INFO << "Destroyed world " << name;
}
vox::world::World* vox::world::Universe::getWorld(const std::string& name) const
{
	const auto& result = m_worlds.find(name);
	return result == m_worlds.end() ? nullptr : result->second.get();
}
std::vector<vox::world::World*> vox::world::Universe::getWorlds() const
{
	std::vector<World*> worlds;
	for (const auto& world : m_worlds)
		worlds.push_back(world.second.get());
	return worlds;
}

void vox::world::Universe::onLoad()
{
	loadRegistry();
	loadBlocks();
}

void vox::world::Universe::loadRegistry()
{
	// Load blocks from the persistent block data file
	hen::io::XMLFile dataFile{ "data/universe/data.xml" };
	const auto& doc = dataFile.open();
	if (const auto& blocks = doc.child("blocks"))
	{
		for (auto& block = blocks.first_child(); block; block = block.next_sibling())
		{
			const std::string name = block.attribute("name").as_string();
			const unsigned int id = block.attribute("id").as_uint(0);
			m_registry.insert(name, id);
		}
	}

	// Load up new blocks from the block folder
	hen::io::Folder blockFolder{ "data/universe/blocks" };
	for (const auto& file : blockFolder.getFiles())
	{
		if (!m_registry.has(file.getName()))
			m_registry.add(file.getName());
	}
}
void vox::world::Universe::loadBlocks()
{
	for (auto& block : m_registry.getBlocks())
		loadBlock(block);
}
void vox::world::Universe::loadBlock(Block& block)
{
	hen::io::XMLFile file{ "data/universe/blocks/" + block.getName() + ".xml" };
	const auto& doc = file.open();

	if (const auto data = doc.child("data"))
	{
		if (const auto light = data.child("light"))
		{
			if (const auto emit = light.attribute("emit"))
				block.setLightEmittance(hen::string::as_uvec4(emit.as_string()));
			if (const auto absorb = light.attribute("absorb"))
				block.setLightAbsorption(hen::string::as_uvec4(absorb.as_string()));
			if (const auto filter = light.attribute("filter"))
				block.setLightFilter(hen::string::as_uvec4(filter.as_string()));
		}
	}
}

