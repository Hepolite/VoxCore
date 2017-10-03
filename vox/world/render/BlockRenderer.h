
#pragma once

#include "vox/world/render/data/BlockModel.h"
#include "vox/world/render/data/BlockTexture.h"
#include "vox/world/render/RenderLayer.h"
#include "vox/world/Side.h"

#include "hen/io/File.h"
#include "hen/render/texture/Texture2DArray.h"

namespace vox
{
	namespace world
	{
		class BlockRegistry;

		namespace render
		{
			class BlockRenderer
			{
			public:
				void onLoad(const hen::io::File& file, const BlockRegistry& registry, hen::texture::Texture2DArray& texture);

				inline const BlockModel& getModel() const { return m_model; }
				inline const BlockTexture& getTexture(const Side& side) const { return m_texture[side.id]; }
				inline RenderLayer getRenderLayer() const { return m_layer; }
				bool isOccluding(const Side& side) const { return m_occlude[side.id]; }

			private:
				BlockModel m_model;
				BlockTexture m_texture[Side::COUNT];
				RenderLayer m_layer;
				bool m_occlude[Side::COUNT];
			};
		}
	}
}