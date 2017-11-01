
#include "VoxScriptData.h"

#include "vox/VoxCore.h"
#include "vox/world/BlockRegistry.h"
#include "vox/world/data/BlockQueryHelper.h"
#include "vox/world/Universe.h"
#include "vox/world/World.h"

#include "hen/logic/script/ScriptHelper.h"

namespace
{
	using namespace vox;
	using namespace vox::data;

	void addBlock(hen::script::ScriptHelper& helper)
	{
		helper.addType<BlockData>("BlockData");
		helper.addConstructor<BlockData(unsigned int)>("BlockData");
		helper.addConstructor<BlockData(unsigned int, glm::ivec4)>("BlockData");

		helper.addFunction(&BlockData::getData, "getData");
		helper.addFunction(&BlockData::getId, "getId");
		helper.addFunction(&BlockData::getLight, "getLight");
		helper.addFunction(&BlockData::setData, "setData");
		helper.addFunction(&BlockData::setId, "setId");
		helper.addFunction(&BlockData::setLight, "setLight");
	}
	void addBlockRegistry(hen::script::ScriptHelper& helper)
	{
		helper.addFunction(&world::BlockRegistry::has, "has");
		helper.addFunction(&world::BlockRegistry::getId, "getId");
		helper.addFunction(&world::BlockRegistry::getName, "getName");
	}
	void addBlockQuery(hen::script::ScriptHelper& helper)
	{
		helper.addType<ChunkQuery>("ChunkQuery");
		helper.addConstructor<ChunkQuery()>("ChunkQuery");
		helper.addFunction<ChunkQuery&, ChunkQuery, ChunkQuery&&>(&ChunkQuery::operator=, "=");

		helper.addFunction(BlockQueryHelper::readBlock, "queryReadBlock");
		helper.addFunction(BlockQueryHelper::writeBlock, "queryWriteBlock");
		helper.addFunction(BlockQueryHelper::readCylinder, "queryReadCylinder");
		helper.addFunction(BlockQueryHelper::writeCylinder, "queryWriteCylinder");
		helper.addFunction(BlockQueryHelper::readEllipse, "queryReadEllipse");
		helper.addFunction(BlockQueryHelper::writeEllipse, "queryWriteEllipse");
		helper.addFunction(BlockQueryHelper::readLine, "queryReadLine");
		helper.addFunction(BlockQueryHelper::writeLine, "queryWriteLine");
		helper.addFunction(BlockQueryHelper::readRectangle, "queryReadRectangle");
		helper.addFunction(BlockQueryHelper::writeRectangle, "queryWriteRectangle");
		helper.addFunction(BlockQueryHelper::readSphere, "queryReadSphere");
		helper.addFunction(BlockQueryHelper::writeSphere, "queryWriteSphere");
	}
	void addWorld(hen::script::ScriptHelper& helper)
	{
		helper.addFunction(&world::World::getBlock, "getBlock");
		helper.addFunction(&world::World::acceptReadQuery, "acceptReadQuery");
		helper.addFunction(&world::World::acceptWriteQuery, "acceptWriteQuery");

		helper.addFunction(&world::World::debugMemusage, "debugWorldMemory");
	}
	void addUniverse(hen::script::ScriptHelper& helper)
	{
		helper.addGlobalVariable(&VoxCore::getUniverse(), "Universe");

		helper.addFunction(&world::Universe::getRegistry, "getRegistry");
		helper.addFunction(&world::Universe::getWorld, "getWorld");
	}
}

vox::script::VoxScriptData::VoxScriptData()
{
	hen::script::ScriptHelper::addScriptData([](hen::script::ScriptHelper& helper)
	{
		addBlock(helper);
		addBlockRegistry(helper);
		addBlockQuery(helper);
		addWorld(helper);
		addUniverse(helper);
	});
}
