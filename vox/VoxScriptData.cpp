
#include "VoxScriptData.h"

#include "vox/VoxCore.h"
#include "vox/world/BlockRegistry.h"
#include "vox/world/Universe.h"
#include "vox/world/World.h"

#include "hen/logic/script/ScriptHelper.h"

vox::script::VoxScriptData::VoxScriptData()
{
	hen::script::ScriptHelper::addScriptData([](hen::script::ScriptHelper& helper)
	{
		//////////////
		// Universe //
		//////////////
		helper.addGlobalVariable(&VoxCore::getUniverse(), "Universe");
		helper.addFunction(&world::Universe::getRegistry, "getRegistry");
		helper.addFunction(&world::Universe::getWorld, "getWorld");

		// World
		helper.addFunction(&world::World::getBlock, "getBlock");
		helper.addFunction(&world::World::setBlock, "setBlock");
		helper.addFunction(&world::World::setBlockCylinder, "setBlockCylinder");
		helper.addFunction(&world::World::setBlockLine, "setBlockLine");
		helper.addFunction(&world::World::setBlockEllipse, "setBlockEllipse");
		helper.addFunction(&world::World::setBlockRectangle, "setBlockRectangle");

		// Block registry
		helper.addFunction(&world::BlockRegistry::getId, "getId");
		helper.addFunction(&world::BlockRegistry::getName, "getName");
	});
}
