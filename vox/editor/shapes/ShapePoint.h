
#pragma once

#include "vox/editor/shapes/Shape.h"

namespace vox
{
	namespace editor
	{
		namespace shape
		{
			class ShapePoint : public Shape
			{
			public:
				ShapePoint() : Shape("point") {}

			private:
				virtual void mesh(std::vector<glm::vec3>& vertices, std::vector<unsigned int>& indices) const final override;
			};
		}
	}
}