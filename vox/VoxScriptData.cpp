
#include "VoxScriptData.h"

#include "vox/editor/tools/ToolWorldReplace.h"
#include "vox/editor/tools/ToolWorldShaping.h"
#include "vox/VoxCore.h"
#include "vox/world/BlockRegistry.h"
#include "vox/world/QueryHelper.h"
#include "vox/world/Universe.h"
#include "vox/world/World.h"

#include "hen/logic/script/ScriptHelper.h"

namespace
{
	using namespace vox;
	using namespace vox::data;
	using namespace vox::world;

	void addBlock(hen::script::ScriptHelper& helper)
	{
		helper.addType<BlockData>("BlockData");
		helper.addConstructor<BlockData()>("BlockData");
		helper.addConstructor<BlockData(unsigned int)>("BlockData");
		helper.addConstructor<BlockData(unsigned int, glm::ivec4)>("BlockData");
		helper.addFunction(&BlockData::operator==, "==");
		helper.addFunction(&BlockData::operator!=, "!=");

		helper.addFunction(&BlockData::getData, "getData");
		helper.addFunction(&BlockData::getId, "getId");
		helper.addFunction(&BlockData::getLight, "getLight");
		helper.addFunction(&BlockData::setData, "setData");
		helper.addFunction(&BlockData::setId, "setId");
		helper.addFunction(&BlockData::setLight, "setLight");
	}
	void addBlockRegistry(hen::script::ScriptHelper& helper)
	{
		helper.addFunction<bool, BlockRegistry, const std::string&>(&BlockRegistry::has, "has");
		helper.addFunction<bool, BlockRegistry, unsigned int>(&BlockRegistry::has, "has");
		helper.addFunction<const Block&, world::BlockRegistry, const std::string&>(&BlockRegistry::getBlock, "getBlock");
		helper.addFunction<const Block&, world::BlockRegistry, unsigned int>(&BlockRegistry::getBlock, "getBlock");
		helper.addFunction(&BlockRegistry::getId, "getId");
		helper.addFunction(&BlockRegistry::getName, "getName");
	}
	void addBlockQuery(hen::script::ScriptHelper& helper)
	{
		helper.addType<ChunkQuery>("ChunkQuery");
		helper.addConstructor<ChunkQuery()>("ChunkQuery");
		helper.addFunction<ChunkQuery&, ChunkQuery, ChunkQuery&&>(&ChunkQuery::operator=, "=");
		helper.addFunction<void, ChunkQuery, const BlockData&, const glm::ivec3&>(&ChunkQuery::add, "add");
		helper.addFunction(&ChunkQuery::has, "has");
		helper.addFunction(&ChunkQuery::get, "get");
		helper.addFunction(&ChunkQuery::iter, "iter");

		helper.addFunction(&ChunkQueryIterator::isValid, "isValid");
		helper.addFunction(&ChunkQueryIterator::next, "next");
		helper.addFunction(&ChunkQueryIterator::getPos, "getPos");
		helper.addFunction(&ChunkQueryIterator::getData, "getData");
	}
	void addWorld(hen::script::ScriptHelper& helper)
	{
		helper.addFunction(&World::getBlock, "getBlock");
		helper.addFunction(&World::acceptReadQuery, "acceptReadQuery");
		helper.addFunction(&World::acceptWriteQuery, "acceptWriteQuery");
	}
	void addUniverse(hen::script::ScriptHelper& helper)
	{
		helper.addGlobalVariable(&VoxCore::getUniverse(), "Universe");

		helper.addFunction(&Universe::getRegistry, "getRegistry");
		helper.addFunction(&Universe::getWorld, "getWorld");

		helper.addFunction(QueryHelper::readBlock, "queryReadBlock");
		helper.addFunction(QueryHelper::writeBlock, "queryWriteBlock");
		helper.addFunction(QueryHelper::readCylinder, "queryReadCylinder");
		helper.addFunction(QueryHelper::writeCylinder, "queryWriteCylinder");
		helper.addFunction(QueryHelper::readEllipse, "queryReadEllipse");
		helper.addFunction(QueryHelper::writeEllipse, "queryWriteEllipse");
		helper.addFunction(QueryHelper::readLine, "queryReadLine");
		helper.addFunction(QueryHelper::writeLine, "queryWriteLine");
		helper.addFunction(QueryHelper::readRectangle, "queryReadRectangle");
		helper.addFunction(QueryHelper::writeRectangle, "queryWriteRectangle");
		helper.addFunction(QueryHelper::readSphere, "queryReadSphere");
		helper.addFunction(QueryHelper::writeSphere, "queryWriteSphere");
	}

	void addToolWorldReplace(hen::script::ScriptHelper& helper)
	{
		helper.addFunction(tool::world::replace, "toolWorldReplace");
	}
	void addToolWorldShaping(hen::script::ScriptHelper& helper)
	{
		helper.addType<tool::world::WorldFilterKernel>("WorldFilterKernel");
		helper.addConstructor<tool::world::WorldFilterKernel()>("WorldFilterKernel");
		helper.addFunction(&tool::world::WorldFilterKernel::add, "add");

		helper.addFunction(tool::world::erode, "toolWorldFilter");
		helper.addFunction(tool::world::erode, "toolWorldErode");
		helper.addFunction(tool::world::smooth, "toolWorldSmooth");

		helper.addFunction(tool::world::pull, "toolWorldPull");
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

		addToolWorldReplace(helper);
		addToolWorldShaping(helper);
	});
}
