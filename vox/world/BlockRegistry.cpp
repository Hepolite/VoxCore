
#include "vox/world/BlockRegistry.h"

#include <Log.h>

bool vox::world::BlockRegistry::has(const std::string & name) const
{
	return m_nameToId.find(name) != m_nameToId.end();
}
bool vox::world::BlockRegistry::add(const std::string& name)
{
	return insert(name, findFreeIndex());
}
bool vox::world::BlockRegistry::insert(const std::string& name, unsigned int id)
{
	if (name == "")
	{
		LOG_WARNING << "Attempted to register block without name!";
		return false;
	}
	if (id == 0 && name != "air")
	{
		LOG_WARNING << "Attempted to register " << name << " at id 0!";
		return false;
	}
	if (id != 0 && name == "air")
	{
		LOG_WARNING << "Attempted to register air at id " << id << "!";
		return false;
	}
	if (getName(id) != "air")
	{
		LOG_WARNING << "The block at index " << id << " has already been registered!";
		return false;
	}
	if (has(name))
	{
		LOG_WARNING << "The block " << name << " has already been registered!";
		return false;
	}

	for (unsigned int i = m_idToName.size(); i <= id; i++)
		m_idToName.emplace_back("");
	m_nameToId[name] = id;
	m_idToName[id] = name;
	LOG_INFO << "Storing block " << name << " at id " << id;
	return true;
}

unsigned int vox::world::BlockRegistry::getId(const std::string& name) const
{
	const auto& result = m_nameToId.find(name);
	return result == m_nameToId.end() ? 0 : result->second;
}
std::string vox::world::BlockRegistry::getName(unsigned int id) const
{
	return id >= m_idToName.size() ? "air" : m_idToName[id];
}

unsigned int vox::world::BlockRegistry::findFreeIndex() const
{
	for (unsigned int i = 0; i < m_idToName.size(); ++i)
	{
		if (m_idToName[i] == "")
			return i;
	}
	return m_idToName.size();
}
