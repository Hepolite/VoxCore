
#pragma once

#include <string>
#include <unordered_map>
#include <vector>

namespace vox
{
	namespace world
	{
		class BlockRegistry
		{
		public:
			bool has(const std::string& name) const;
			bool add(const std::string& name);
			bool insert(const std::string& name, unsigned int id);

			unsigned int getId(const std::string& name) const;
			std::string getName(unsigned int id) const;

			inline unsigned int size() const { return m_idToName.size(); }

		private:
			unsigned int findFreeIndex() const;

			std::unordered_map<std::string, unsigned int> m_nameToId;
			std::vector<std::string> m_idToName;
		};
	}
}