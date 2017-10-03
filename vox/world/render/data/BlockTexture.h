
#pragma once

#include "vox/chunk/BlockVolume.h"
#include "vox/world/Side.h"

#include "hen/io/File.h"
#include "hen/render/texture/Texture2DArray.h"

#include <extern/pugixml/pugixml.hpp>
#include <glm/vec3.hpp>

#include <functional>

namespace vox
{
	namespace world
	{
		class BlockRegistry;

		namespace render
		{
			class BlockTexture
			{
			public:
				void onLoad(const hen::io::File& file, const BlockRegistry& registry, hen::texture::Texture2DArray& texture);

				inline unsigned int getHandle() const { return m_handle; }
				inline unsigned int getTexture(const chunk::BlockVolume& volume, const glm::ivec3& pos, const Side& side) const
					{ return m_lookup(volume, pos, side); }

			private:
				void loadDefault();
				void loadConnected(const pugi::xml_node& node, const BlockRegistry& registry);
				void loadRandom(const pugi::xml_node& node);

				std::function<unsigned int(const chunk::BlockVolume&, const glm::ivec3&, const Side&)> m_lookup =
					[](const chunk::BlockVolume&, const glm::ivec3&, const Side&) { return 0; };

				unsigned int m_handle = 0;
			};
		}
	}
}