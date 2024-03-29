#ifndef SMOL_BLIT2D_RECT_DEFINED
#define SMOL_BLIT2D_RECT_DEFINED


#include "vec2.hpp"


namespace Smol::Blit2D
{
	template<typename T>
	struct Rect
	{
		Vec2<T> pos;
		Vec2<T> size;
	};
	
	using RectF = Rect<float>;
	using RectI = Rect<int>;
}


#endif // SMOL_BLIT2D_RECT_DEFINED