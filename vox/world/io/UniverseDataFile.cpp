
#include "vox/world/io/UniverseDataFile.h"

#include "vox/events/Chunk.h"
#include "vox/events/World.h"

#include "hen/core/Core.h"
#include "hen/event/EventBus.h"

vox::world::io::UniverseDataFile::UniverseDataFile()
{
	auto& eventBus = hen::Core::getEventBus();
	m_worldCreateListener = eventBus.registerCallback<events::WorldCreate>([this](events::WorldCreate& event)
	{
		load(event.getWorld());
		save(event.getWorld());
	});
	m_worldDestroyListener = eventBus.registerCallback<events::WorldDestroy>([this](events::WorldDestroy& event)
	{
		close(event.getWorld());
	});
	m_chunkBlockChangeListener = eventBus.registerCallback<events::ChunkBlockChange>([this](events::ChunkBlockChange& event)
	{
		const auto& saver = m_savers.find(event.getWorld());
		if (saver != m_savers.end())
			saver->second.markRegionForStorage(event.getChunkPos() >> 4);
	});
}
vox::world::io::UniverseDataFile::~UniverseDataFile()
{
	auto& eventBus = hen::Core::getEventBus();
	eventBus.unregisterListener(m_worldCreateListener);
	eventBus.unregisterListener(m_worldDestroyListener);
	eventBus.unregisterListener(m_chunkBlockChangeListener);
}

void vox::world::io::UniverseDataFile::onProcess()
{
	for (auto& loader : m_loaders)
		loader.second.onProcess();
}

void vox::world::io::UniverseDataFile::load(world::World* world)
{
	m_loaders.emplace(std::piecewise_construct, std::make_tuple(world), std::make_tuple(world));
}
void vox::world::io::UniverseDataFile::save(world::World* world)
{
	m_savers.emplace(std::piecewise_construct, std::make_tuple(world), std::make_tuple(world));
}
void vox::world::io::UniverseDataFile::close(world::World* world)
{
	m_loaders.erase(world);
	m_savers.erase(world);
}
