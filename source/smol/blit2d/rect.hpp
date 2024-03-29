#ifndef SMOL_BLIT2D_RECT_DEFINED
#define SMOL_BLIT2D_RECT_DEFINED


#include "vec2.hpp"
#include "size.hpp"


namespace Smol::Blit2D
{
	template<typename T>
	struct Rect
	{
		T l;
		T t;
		T r;
		T b;
		
		
		constexpr Rect() noexcept:
			l(0), t(0), r(0), b(0)
		{}
		
		constexpr Rect(T l, T t, T r, T b) noexcept:
			l(l), t(t), r(r), b(b)
		{}
		
		constexpr Rect(Vec2<T> pos, Size<T> size) noexcept:
			l(pos.x), t(pos.y), r(pos.x + size.w), b(pos.y + size.h)
		{}
		
		
		constexpr Size<T> GetSize() const
		{ return { r - l, b - t }; }
	};
	
	
	using RectF = Rect<float>;
	using RectI = Rect<int>;
}


#endif // SMOL_BLIT2D_RECT_DEFINED