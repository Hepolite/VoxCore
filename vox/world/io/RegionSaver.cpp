
#include "vox/world/io/RegionSaver.h"

#include "hen/io/File.h"

#include <fstream>
#include <ostream>

vox::world::io::RegionSaver::RegionSaver(const world::World* world, const glm::ivec3& pos)
	: m_world(world), m_pos(pos)
{}

void vox::world::io::RegionSaver::saveToDisk()
{
	hen::io::File{ getFilepath() }.erase();

	std::ofstream stream{ getFilepath() };
	if (stream.is_open())
	{
		glm::ivec3 pos;
		for (pos.x = 0; pos.x < 16; ++pos.x)
		for (pos.y = 0; pos.y < 16; ++pos.y)
		for (pos.z = 0; pos.z < 16; ++pos.z)
		{
			const auto chunk = m_world->getChunk(m_pos * 16 + pos);
			if (chunk == nullptr)
				continue;
			const auto data = chunk->getStoringData();
			if (data.empty())
				continue;

			stream << pos.x << " " << pos.y << " " << pos.z << " " << data.getRawData().size() << " ";
			for (const auto& node : data.getRawData())
				stream << std::to_string(node.first.getData()) << " " << std::to_string(node.second) << " ";
			stream << std::endl;
		}
	}
	stream.close();
}

std::string vox::world::io::RegionSaver::getFilepath() const
{
	return "data/universe/worlds/" + m_world->getName() + "/r." + std::to_string(m_pos.x) + "." + std::to_string(m_pos.y) + "." + std::to_string(m_pos.z) + ".txt";
}
