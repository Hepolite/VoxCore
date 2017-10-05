
#include "vox/world/Universe.h"

#include "vox/world/data/BlockQueryHelper.h"

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
	LOG_INFO << "Created world " << name;
	return ptr;
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
	loadBlocks();
	loadWorlds();
}
void vox::world::Universe::loadBlocks()
{
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

	hen::io::Folder blockFolder{ "data/universe/blocks" };
	for (const auto& file : blockFolder.getFiles())
	{
		if (!m_registry.has(file.getName()))
			m_registry.add(file.getName());
	}
}
void vox::world::Universe::loadWorlds()
{
	hen::io::Folder worldFolder{ "data/universe/worlds" };
	for (const auto& file : worldFolder.getFiles())
	{
		auto world = createWorld(file.getName());

		hen::io::XMLFile xmlFile{ file };
		const auto& doc = xmlFile.open();
		for (auto& data = doc.child("data").first_child(); data; data = data.next_sibling())
		{
			const std::string name = data.name();
			const data::BlockData id{ m_registry.getId(data.attribute("type").as_string()), glm::ivec4{} };
			const glm::ivec3 start = hen::string::as_ivec3(data.attribute("start").as_string());

			data::ChunkWriteQuery query;
			if (name == "point")
				query = data::BlockQueryHelper::writeBlock(id, start);
			else if (name == "line")
				query = data::BlockQueryHelper::writeLine(id, start, hen::string::as_ivec3(data.attribute("end").as_string()));
			else if (name == "rectangle")
				query = data::BlockQueryHelper::writeRectangle(id, start, hen::string::as_ivec3(data.attribute("end").as_string()));
			else if (name == "ellipse")
				query = data::BlockQueryHelper::writeEllipse(id, start, hen::string::as_ivec3(data.attribute("end").as_string()));
			else if (name == "sphere")
				query = data::BlockQueryHelper::writeSphere(id, start, data.attribute("radius").as_int());
			world->acceptQuery(query);
		}
	}
}
