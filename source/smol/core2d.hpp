#ifndef SMOL_CORE2D_DEFINED
#define SMOL_CORE2D_DEFINED


#include "core2d/vec2.hpp"
#include "core2d/size.hpp"
#include "core2d/rect.hpp"


namespace Smol
{
	using Vec2I = Core2D::Vec2<int>;
	using Vec2F = Core2D::Vec2<float>;
	
	using SizeI = Core2D::Size<int>;
	using SizeF = Core2D::Size<float>;
	
	using RectI = Core2D::Rect<int>;
	using RectF = Core2D::Rect<float>;
}


#endif // SMOL_CORE2D_DEFINED