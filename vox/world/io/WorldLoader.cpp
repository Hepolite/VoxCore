
#include "vox/world/io/WorldLoader.h"

#include "hen/io/Folder.h"
#include "hen/util/StringLib.h"

#include <fstream>
#include <ostream>
#include <unordered_set>

vox::world::io::WorldLoader::WorldLoader(world::World* world)
	: m_world(world)
{
	m_thread = std::thread{ [this]() { doWork(); } };
}
vox::world::io::WorldLoader::~WorldLoader()
{
	m_thread.join();
}

void vox::world::io::WorldLoader::onProcess()
{
	std::lock_guard<std::mutex> guard{ m_mutex };
	std::unordered_set<glm::ivec3> finishedRegions;

	for (auto& it : m_regions)
	{
		auto& region = it.second;

		glm::uvec3 pos;
		data::ChunkDataRLE data;
		while (region.pollData(pos, data))
			m_world->injectChunkStorageData(it.first * 16 + glm::ivec3{ pos }, std::move(data));
		if (!region.hasData())
			finishedRegions.insert(it.first);
	}

	for (const auto& pos : finishedRegions)
		m_regions.erase(pos);
}

bool vox::world::io::WorldLoader::isDone() const
{
	std::lock_guard<std::mutex> guard{ m_mutex };
	return m_done && m_regions.empty();
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
		{
			std::lock_guard<std::mutex> guard{ m_mutex };
			const auto& it = m_regions.emplace(std::piecewise_construct, std::make_tuple(pos), std::make_tuple(m_world, pos));
		}
		m_regions.find(pos)->second.loadFromDisk();
	}
	m_done = true;
}
