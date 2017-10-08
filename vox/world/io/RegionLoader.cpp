
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
		glm::uvec3 pos;
		unsigned int size, block, count;
		while (stream >> pos.x >> pos.y >> pos.z >> size)
		{
			data::ChunkDataRLE data;
			for (unsigned int i = 0; i < size; ++i)
			{
				stream >> block >> count;
				data.getRawData().emplace_back(block, count);
			}
			pushData(pos, std::move(data));
		}
	}
	stream.close();
	m_done = true;
}

bool vox::world::io::RegionLoader::hasData() const
{
	std::lock_guard<std::mutex> guard{ m_mutex };
	return !m_done || !m_data.empty();
}
bool vox::world::io::RegionLoader::pollData(glm::uvec3& pos, data::ChunkDataRLE& data)
{
	std::lock_guard<std::mutex> guard{ m_mutex };
	if (m_data.empty())
		return false;
	const auto& first = m_data.begin();
	pos = first->first;
	std::swap(data, first->second);
	m_data.erase(pos);
	return true;
}
void vox::world::io::RegionLoader::pushData(const glm::uvec3& pos, data::ChunkDataRLE&& data)
{
	std::lock_guard<std::mutex> guard{ m_mutex };
	m_data.emplace(pos, std::move(data));
}

std::string vox::world::io::RegionLoader::getFilepath() const
{
	return "data/universe/worlds/" + m_world->getName() + "/r." + std::to_string(m_pos.x) + "." + std::to_string(m_pos.y) + "." + std::to_string(m_pos.z) + ".txt";
}
