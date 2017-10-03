
#include "vox/world/render/BlockRenderer.h"

#include "hen/io/XMLFile.h"

#include <Log.h>

void vox::world::render::BlockRenderer::onLoad(
	const hen::io::File& file,
	const BlockRegistry& registry,
	hen::texture::Texture2DArray& texture
){
	hen::io::XMLFile xmlFile{ file };
	const auto& doc = xmlFile.open();
	const auto& visual = doc.child("visual");

	if (const auto& modelNode = visual.child("model"))
		m_model.onLoad(modelNode.attribute("file").as_string());

	if (const auto& textureNode = visual.child("texture"))
	{
		const std::string def = textureNode.attribute("file").as_string();
		const std::string side = textureNode.child("side").attribute("file").as_string(def.c_str());
		const std::string north = textureNode.child("north").attribute("file").as_string(side.c_str());
		const std::string east = textureNode.child("east").attribute("file").as_string(side.c_str());
		const std::string south = textureNode.child("south").attribute("file").as_string(side.c_str());
		const std::string west = textureNode.child("west").attribute("file").as_string(side.c_str());
		const std::string top = textureNode.child("top").attribute("file").as_string(def.c_str());
		const std::string bottom = textureNode.child("bottom").attribute("file").as_string(def.c_str());
		const std::string other = textureNode.child("other").attribute("file").as_string(def.c_str());

		if (!north.empty())
			m_texture[Side::NORTH.id].onLoad(north, registry, texture);
		if (!east.empty())
			m_texture[Side::EAST.id].onLoad(east, registry, texture);
		if (!south.empty())
			m_texture[Side::SOUTH.id].onLoad(south, registry, texture);
		if (!west.empty())
			m_texture[Side::WEST.id].onLoad(west, registry, texture);
		if (!top.empty())
			m_texture[Side::TOP.id].onLoad(top, registry, texture);
		if (!bottom.empty())
			m_texture[Side::BOTTOM.id].onLoad(bottom, registry, texture);
		if (!other.empty())
			m_texture[Side::UNKNOWN.id].onLoad(other, registry, texture);
	}

	if (const auto& occlusionNode = visual.child("occlude"))
	{
		for (auto& side = occlusionNode.first_attribute(); side; side = side.next_attribute())
			m_occlude[Side::fromName(side.name()).id] = side.as_bool(true);
	}

	if (const auto& renderNode = visual.child("render"))
	{
		const std::string layer = renderNode.attribute("layer").as_string("opaque");
		if (layer == "cutout")
			m_layer = render::RenderLayer::CUTOUT;
		else if (layer == "transparent")
			m_layer = render::RenderLayer::TRANSPARENT;
		else if (layer == "invisible")
			m_layer = render::RenderLayer::INVISIBLE;
		else if (layer == "opaque")
			m_layer = render::RenderLayer::OPAQUE;
		else
			LOG(plog::warning) << "Invalid render layer " << layer << " in file " << file.getPath();
	}
}
