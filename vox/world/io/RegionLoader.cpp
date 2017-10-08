
#include "vox/world/io/RegionLoader.h"

#include <fstream>
#include <ostream>

vox::world::io::RegionLoader::RegionLoader(world::World* world, const glm::ivec3& pos)
	: m_world(world), m_pos(pos)
{}

void vox::world::io::RegionLoader::loadFromDisk()
{
	std::ifstream stream{ getFilepath() };
	if (stream.is_open())
	{
		glm::ivec3 pos;
		unsigned int size, block, count;
		while (stream >> pos.x >> pos.y >> pos.z >> size)
		{
			data::ChunkDataRLE data;
			for (unsigned int i = 0; i < size; ++i)
			{
				stream >> block >> count;
				data.getRawData().emplace_back(block, count);
			}
			m_world->getOrCreateChunk(m_pos * 16 + pos).setChunkData(std::move(data));
		}
	}
	stream.close();
}

std::string vox::world::io::RegionLoader::getFilepath() const
{
	return "data/universe/worlds/" + m_world->getName() + "/r." + std::to_string(m_pos.x) + "." + std::to_string(m_pos.y) + "." + std::to_string(m_pos.z) + ".txt";
}
