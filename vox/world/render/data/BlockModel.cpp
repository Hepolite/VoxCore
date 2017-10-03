
#include "vox/world/render/data/BlockModel.h"

#include "hen/io/XMLFile.h"
#include "hen/util/StringLib.h"

void vox::world::render::BlockModel::onLoad(const hen::io::File& file)
{
	hen::io::XMLFile xmlFile{ "data/universe/blocks/models/" + file.getPath() };
	const auto& doc = xmlFile.open();

	const auto& geometry = doc.child("geometry");
	if (geometry)
	{
		for (auto& face = geometry.first_child(); face; face = face.next_sibling())
		{
			const auto& side = Side::fromName(face.name());

			const auto& vertices = face.child("vertices");
			for (auto& vertex = vertices.first_child(); vertex; vertex = vertex.next_sibling())
			{
				const auto& pos = hen::string::as_vec3(vertex.attribute("position").as_string());
				const auto& nor = hen::string::as_vec3(vertex.attribute("normal").as_string());
				const auto& uv = hen::string::as_vec3(vertex.attribute("uv").as_string());
				const auto& col = hen::string::as_vec3(vertex.attribute("color").as_string());
				m_vertices[side.id].emplace_back(pos, nor, uv, col);
			}

			for (const auto& index : hen::string::as_int_vector(face.child_value("indices")))
				m_indices[side.id].emplace_back(index);
		}
	}
}
