
#include "vox/world/io/WorldSaver.h"

vox::io::WorldSaver::WorldSaver()
{
}
vox::io::WorldSaver::~WorldSaver()
{
}

void vox::io::WorldSaver::onProcess()
{
	doWork();
}
void vox::io::WorldSaver::doWork()
{
}
void vox::io::WorldSaver::clearWork()
{
}

void vox::io::WorldSaver::storeChunk(const world::Chunk& chunk)
{
}
void vox::io::WorldSaver::removeChunk(const world::Chunk& chunk)
{
}

