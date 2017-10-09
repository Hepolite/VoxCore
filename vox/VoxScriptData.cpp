
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

	void addBlock(hen::script::ScriptHelper& helper)
	{
		helper.addType<data::BlockData>("BlockData");
		helper.addConstructor<data::BlockData(unsigned int)>("BlockData");
		helper.addConstructor<data::BlockData(unsigned int, glm::ivec4)>("BlockData");

		helper.addFunction(&data::BlockData::getData, "getData");
		helper.addFunction(&data::BlockData::getId, "getId");
		helper.addFunction(&data::BlockData::getLight, "getLight");
		helper.addFunction(&data::BlockData::setData, "setData");
		helper.addFunction(&data::BlockData::setId, "setId");
		helper.addFunction(&data::BlockData::setLight, "setLight");
	}
	void addBlockRegistry(hen::script::ScriptHelper& helper)
	{
		helper.addFunction(&world::BlockRegistry::has, "has");
		helper.addFunction(&world::BlockRegistry::getId, "getId");
		helper.addFunction(&world::BlockRegistry::getName, "getName");
	}
	void addBlockQuery(hen::script::ScriptHelper& helper)
	{
		helper.addType<data::ChunkReadQuery>("ChunkReadQuery");
		helper.addType<data::ChunkWriteQuery>("ChunkWriteQuery");
		helper.addConstructor<data::ChunkReadQuery()>("ChunkReadQuery");
		helper.addConstructor<data::ChunkWriteQuery()>("ChunkWriteQuery");
		helper.addFunction<data::ChunkReadQuery&, data::ChunkReadQuery, data::ChunkReadQuery&&>(&data::ChunkReadQuery::operator=, "=");
		helper.addFunction<data::ChunkWriteQuery&, data::ChunkWriteQuery, data::ChunkWriteQuery&&>(&data::ChunkWriteQuery::operator=, "=");

		helper.addFunction(data::BlockQueryHelper::readBlock, "queryReadBlock");
		helper.addFunction(data::BlockQueryHelper::writeBlock, "queryWriteBlock");
		helper.addFunction(data::BlockQueryHelper::writeCylinder, "queryWriteCylinder");
		helper.addFunction(data::BlockQueryHelper::writeEllipse, "queryWriteEllipse");
		helper.addFunction(data::BlockQueryHelper::writeLine, "queryWriteLine");
		helper.addFunction(data::BlockQueryHelper::writeRectangle, "queryWriteRectangle");
		helper.addFunction(data::BlockQueryHelper::writeSphere, "queryWriteSphere");
	}
	void addWorld(hen::script::ScriptHelper& helper)
	{
		helper.addFunction(&world::World::getBlock, "getBlock");
		helper.addFunction<void, world::World, data::ChunkReadQuery&>(&world::World::acceptQuery, "acceptQuery");
		helper.addFunction<void, world::World, data::ChunkWriteQuery&>(&world::World::acceptQuery, "acceptQuery");

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
