
#pragma once

#include "vox/world/Block.h"

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
			bool add(const std::string& name);
			bool insert(const std::string& name, unsigned int id);

			bool has(const std::string& name) const;
			bool has(unsigned int id) const;

			const Block& getBlock(const std::string& name) const;
			const Block& getBlock(unsigned int id) const;
			inline auto& getBlocks() { return m_blocks; }
			inline auto& getBlocks() const { return m_blocks; }

			inline unsigned int getId(const std::string& name) const { return getBlock(name).getId(); }
			inline std::string getName(unsigned int id) const { return getBlock(id).getName(); }

			inline unsigned int size() const { return m_blocks.size(); }

		private:
			unsigned int findFreeIndex() const;

			std::vector<Block> m_blocks;
			std::unordered_map<std::string, unsigned int> m_blockMap;
		};
	}
}