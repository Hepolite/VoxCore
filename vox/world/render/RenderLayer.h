
#pragma once

namespace vox
{
	namespace world
	{
		namespace render
		{
			enum RenderLayer : unsigned int
			{
				OPAQUE,			// Everything on this layer must have an alpha component of 1.0f
				CUTOUT,			// Everything on this layer must have alpha components of only 0.0f and/or 1.0f
				TRANSPARENT,	// Everything on this layer may have an alpha component of less than 1.0f
				
				COUNT,			// Number of unique visible layers available
				
				INVISIBLE,		// Everything on this layer is invisible
			};
		}
	}
}