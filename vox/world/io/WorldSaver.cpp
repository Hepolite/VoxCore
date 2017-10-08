
#include "vox/world/io/WorldSaver.h"

vox::world::io::WorldSaver::WorldSaver(world::World* world)
	: m_world(world)
{
	m_thread = std::thread{ [this]() { workThread(); } };
}
vox::world::io::WorldSaver::~WorldSaver()
{
	m_running = false;
	m_thread.join();
	finishWork();
}

void vox::world::io::WorldSaver::markRegionForStorage(const glm::ivec3& region)
{
	std::lock_guard<std::mutex> guard{ m_mutex };
	m_regionsToSave.insert(region);
}

void vox::world::io::WorldSaver::grabWork()
{
	std::lock_guard<std::mutex> guard{ m_mutex };
	if (m_saver == nullptr && !m_regionsToSave.empty())
	{
		const auto pos = *m_regionsToSave.begin();
		m_saver = std::make_unique<RegionSaver>(m_world, pos);
		m_regionsToSave.erase(pos);
	}
}
void vox::world::io::WorldSaver::workThread()
{
	while (m_running)
	{
		grabWork();
		doWork();
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
}
void vox::world::io::WorldSaver::doWork()
{
	if (m_saver != nullptr)
		m_saver->saveToDisk();
	m_saver = nullptr;
}
void vox::world::io::WorldSaver::finishWork()
{
	while (!m_regionsToSave.empty())
	{
		grabWork();
		doWork();
	}
}
