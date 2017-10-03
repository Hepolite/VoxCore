
#pragma once

#include "vox/editor/shapes/Shape.h"

#include "hen/util/MathEnum.h"

namespace vox
{
	namespace editor
	{
		namespace shape
		{
			class ShapeCylinder : public Shape
			{
			public:
				ShapeCylinder() : Shape("cylinder") {}

				inline void setAxis(hen::math::Axis axis) { m_axis = axis; update(); }
				inline hen::math::Axis getAxis() const { return m_axis; }

				inline bool isHollow() const { return m_isHollow; }
				inline void setHollow(bool hollow) { m_isHollow = hollow; update(); }

			private:
				virtual void mesh(std::vector<glm::vec3>& vertices, std::vector<unsigned int>& indices) const final override;

				hen::math::Axis m_axis = hen::math::Axis::Z;
				bool m_isHollow = false;
			};
		}
	}
}