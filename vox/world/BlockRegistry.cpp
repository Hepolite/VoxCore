
#include "vox/world/BlockRegistry.h"

#include <Log.h>

bool vox::world::BlockRegistry::add(const std::string& name)
{
	return insert(name, findFreeIndex());
}
bool vox::world::BlockRegistry::insert(const std::string& name, unsigned int id)
{
	if (name.empty())
		LOG_WARNING << "Attempted to register block without name!";
	else if (id == 0 && name != "air")
		LOG_WARNING << "Attempted to register " << name << " at id 0, only air can be registered at index 0!";
	else if (id != 0 && name == "air")
		LOG_WARNING << "Attempted to register air at id " << id << ", air can only be registered at index 0!";
	else if (has(id))
		LOG_WARNING << "The block at index " << id << " (" << getBlock(id).getName() << ") has already been registered!";
	else if (has(name))
		LOG_WARNING << "The block " << name << " has already been registered at index " << getBlock(name).getId() << "!";
	else
	{
		for (unsigned int i = size(); i <= id; i++)
			m_blocks.emplace_back("", 0);
		m_blocks[id] = Block{ name, id };
		m_blockMap[name] = id;
		LOG_INFO << "Storing block " << name << " at id " << id;
		return true;
	}
	return false;
}

bool vox::world::BlockRegistry::has(const std::string& name) const
{
	return m_blockMap.find(name) != m_blockMap.end();
}
bool vox::world::BlockRegistry::has(unsigned int id) const
{
	return id >= size() ? false : !getBlock(id).getName().empty();
}

const vox::world::Block& vox::world::BlockRegistry::getBlock(const std::string& name) const
{
	const auto result = m_blockMap.find(name);
	return getBlock(result == m_blockMap.end() ? 0 : result->second);
}
const vox::world::Block& vox::world::BlockRegistry::getBlock(unsigned int id) const
{
	return m_blocks[id >= size() ? 0 : id];
}

unsigned int vox::world::BlockRegistry::findFreeIndex() const
{
	for (unsigned int i = 0; i < m_blocks.size(); ++i)
	{
		if (!has(i))
			return i;
	}
	return m_blocks.size();
}
