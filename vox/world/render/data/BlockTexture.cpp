
#include "vox/world/render/data/BlockTexture.h"

#include "vox/world/BlockRegistry.h"

#include "hen/util/MathLib.h"
#include "hen/util/StringLib.h"

#include "hen/io/XMLFile.h"

namespace
{
	inline unsigned int mapAll(const glm::ivec3& pos, const vox::world::Side& side, const vox::data::BlockRegion& data, const std::vector<bool>& keys)
	{
		const static unsigned int mappings[] = {
			0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
			0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
			0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
			0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
			0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
			0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
			0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
			0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
			0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
			0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
			0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
			0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
			0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
			0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
			0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
			0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
		};

		return mappings[
			keys[data.getBlock(pos - side.x + side.y).getId()] << 0 |
			keys[data.getBlock(pos + side.y).getId()] << 1 |
			keys[data.getBlock(pos + side.x + side.y).getId()] << 2 |
			keys[data.getBlock(pos + side.x).getId()] << 3 |
			keys[data.getBlock(pos + side.x - side.y).getId()] << 4 |
			keys[data.getBlock(pos - side.y).getId()] << 5 |
			keys[data.getBlock(pos - side.x - side.y).getId()] << 6 |
			keys[data.getBlock(pos - side.x).getId()] << 7
		];
	}
	inline unsigned int mapHorizontal(const glm::ivec3& pos, const vox::world::Side& side, const vox::data::BlockRegion& data, const std::vector<bool>& keys)
	{
		const static unsigned int mappings[] = { 0, 1, 3, 2 };

		return mappings[
			keys[data.getBlock(pos - side.x).getId()] << 0 |
			keys[data.getBlock(pos + side.x).getId()] << 1
		];
	}
	inline unsigned int mapVertical(const glm::ivec3& pos, const vox::world::Side& side, const vox::data::BlockRegion& data, const std::vector<bool>& keys)
	{
		const static unsigned int mappings[] = { 0, 3, 1, 2 };

		return mappings[
			keys[data.getBlock(pos + side.y).getId()] << 0 |
			keys[data.getBlock(pos - side.y).getId()] << 1
		];
	}
}

void vox::world::render::BlockTexture::onLoad(
	const hen::io::File& file,
	const BlockRegistry& registry,
	hen::texture::Texture2DArray& texture
){
	if (file.getType() != "xml")
	{
		m_handle = texture.attach(file);
		loadDefault();
		return;
	}

	hen::io::XMLFile xmlFile{ "data/universe/blocks/textures/" + file.getPath() };
	const auto& doc = xmlFile.open();
	const auto& tex = doc.child("texture");
	const std::string connection = tex.attribute("connection").as_string();
	const std::string path = tex.attribute("file").as_string();

	m_handle = texture.attach(path);
	if (connection == "connected")
		loadConnected(tex, registry);
	else if (connection == "random")
		loadRandom(tex);
	else
		loadDefault();
}

void vox::world::render::BlockTexture::loadDefault()
{
	const auto texture = m_handle | (1u << 16u);
	m_lookup = [texture](const data::BlockRegion&, const glm::ivec3&, const Side&) { return texture; };
}
void vox::world::render::BlockTexture::loadConnected(const pugi::xml_node& node, const BlockRegistry& registry)
{
	const glm::uvec4 start = hen::string::as_uvec4(node.child("indices").attribute("start").as_string("0 1 2 3"));
	const glm::uvec4 end = hen::string::as_uvec4(node.child("indices").attribute("end").as_string());
	const glm::uvec4 difference = end - start + 1u;
	const auto texture = (m_handle + start) | (difference << 16u);

	std::vector<bool> keys;
	keys.resize(registry.size());
	const auto& names = node.child("blocks").attribute("names").as_string();
	for (const auto& name : hen::string::as_string_vector(names))
		keys[registry.getId(name)] = true;

	const std::string direction = node.child("type").attribute("direction").as_string();
	if (direction == "horizontal")
		m_lookup = [texture, keys](const data::BlockRegion& data, const glm::ivec3& pos, const Side& side) { return texture[mapHorizontal(pos, side, data, keys)]; };
	else if (direction == "vertical")
		m_lookup = [texture, keys](const data::BlockRegion& data, const glm::ivec3& pos, const Side& side) { return texture[mapVertical(pos, side, data, keys)]; };
	else
		m_lookup = [texture = m_handle, keys](const data::BlockRegion& data, const glm::ivec3& pos, const Side& side) { return texture + mapAll(pos, side, data, keys); };
}
void vox::world::render::BlockTexture::loadRandom(const pugi::xml_node& node)
{
	const unsigned int start = node.child("indices").attribute("start").as_uint();
	const unsigned int end = node.child("indices").attribute("end").as_uint(start);
	const unsigned int difference = end - start + 1u;
	const auto texture = (m_handle + start) | (difference << 16u);

	m_lookup = [texture](const data::BlockRegion&, const glm::ivec3&, const Side&) { return texture; };
}
