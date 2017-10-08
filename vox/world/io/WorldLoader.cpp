
#include "vox/world/io/WorldLoader.h"

#include "vox/world/io/RegionLoader.h"

#include "hen/io/Folder.h"
#include "hen/util/StringLib.h"

#include <fstream>
#include <ostream>

vox::world::io::WorldLoader::WorldLoader(world::World* world)
	: m_world(world)
{
	m_thread = std::thread{ [this]() { doWork(); } };
}
vox::world::io::WorldLoader::~WorldLoader()
{
	m_thread.join();
}

void vox::world::io::WorldLoader::doWork()
{
	hen::io::Folder folder{ "data/universe/worlds/" + m_world->getName() };
	for (const auto& file : folder.getFiles())
	{
		const auto parts = hen::string::split(file.getName(), '.');
		if (parts.size() != 4)
			continue;
		glm::ivec3 pos{ hen::string::as_int(parts[1]) , hen::string::as_int(parts[2]) , hen::string::as_int(parts[3]) };
		RegionLoader{ m_world, pos }.loadFromDisk();
	}
}
