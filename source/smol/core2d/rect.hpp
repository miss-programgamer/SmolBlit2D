#ifndef SMOL_CORE2D_RECT_DEFINED
#define SMOL_CORE2D_RECT_DEFINED


#include "vec2.hpp"
#include "size.hpp"


namespace Smol
{
	// A 2D rectangle represented by its outer bounds.
	template<typename T>
	struct Rect
	{
		T l;
		T t;
		T r;
		T b;
		
		
		// Construct an empty rectangle.
		constexpr Rect() noexcept:
			l(0), t(0), r(0), b(0)
		{}
		
		// Construct a rectangle out of four bounds.
		constexpr Rect(T l, T t, T r, T b) noexcept:
			l(l), t(t), r(r), b(b)
		{}
		
		// Construct a rectangle out of a position and size.
		constexpr Rect(Vec2<T> pos, Size<T> size) noexcept:
			l(pos.x), t(pos.y), r(pos.x + size.w), b(pos.y + size.h)
		{}
		
		
		// Get this rectangle's size.
		constexpr Size<T> GetSize() const
		{ return { r - l, b - t }; }
	};
	
	
	using RectF = Rect<float>;
	using RectI = Rect<int>;
}


#endif // SMOL_CORE2D_RECT_DEFINED