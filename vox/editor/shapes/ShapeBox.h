
#pragma once

#include "vox/editor/shapes/Shape.h"

namespace vox
{
	namespace editor
	{
		namespace shape
		{
			class ShapeBox : public Shape
			{
			public:
				ShapeBox() : Shape("box") {}

				inline bool isHollow() const { return m_isHollow; }
				inline void setHollow(bool hollow) { m_isHollow = hollow; update(); }

			private:
				virtual void mesh(std::vector<glm::vec3>& vertices, std::vector<unsigned int>& indices) const final override;

				bool m_isHollow = false;
			};
		}
	}
}