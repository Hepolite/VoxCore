
#include "vox/VoxCore.h"

#include "vox/VoxScriptData.h"
#include "vox/world/Universe.h"
#include "vox/world/io/UniverseDataFile.h"
#include "vox/world/render/UniverseRenderer.h"

#include <Log.h>

class vox::VoxCore::Internal
{
public:
	world::Universe m_universe;
	world::io::UniverseDataFile m_universeDataFile;
	world::render::UniverseRenderer m_universeRenderer;

	script::VoxScriptData m_scriptData;
};

vox::VoxCore::Internal* vox::VoxCore::m_internal = nullptr;

vox::VoxCore::VoxCore()
{
	LOG_INFO << "Setting up core voxel resources...";
	m_internal = new Internal();

	LOG_INFO << "Initializing universe...";
	m_internal->m_universe.onLoad();
	LOG_INFO << "Loading universe assets...";
	m_internal->m_universeRenderer.onLoad(getUniverse().getRegistry());
	LOG_INFO << "Done initializing universe!";

	LOG_INFO << "Finished setting up core voxel resources!";
}
vox::VoxCore::~VoxCore()
{
	delete m_internal;
}

vox::world::Universe& vox::VoxCore::getUniverse()
{
	return m_internal->m_universe;
}
vox::world::render::UniverseRenderer& vox::VoxCore::getUniverseRenderer()
{
	return m_internal->m_universeRenderer;
}