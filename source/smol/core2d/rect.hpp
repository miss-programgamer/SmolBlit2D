#ifndef SMOL_CORE2D_RECT_DEFINED
#define SMOL_CORE2D_RECT_DEFINED


#include "vec2.hpp"
#include "size.hpp"


namespace Smol::Core2D
{
	// A 2D rectangle represented by its outer bounds.
	template<typename T>
	struct Rect
	{
		// Left bound.
		T l;
		
		// Top bound.
		T t;
		
		// Right bound.
		T r;
		
		// Bottom bound.
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
		
		// Construct a rectangle out of its top-left and bottom-right corners.
		constexpr Rect(Vec2<T> min, Vec2<T> max) noexcept:
			l(min.x), t(min.y), r(max.x), b(max.y)
		{}
		
		
		// Check whether this rect contains a given point.
		constexpr bool Contains(const Vec2<T>& pos) const
		{ return l <= pos.x && pos.x < r && t <= pos.y && pos.y < b; }
		
		// Check whether this rect overlaps another.
		constexpr bool Overlaps(const Rect<T>& other) const
		{ return !(l >= other.r || other.l >= r || t >= other.b || other.t >= b); }
		
		
		// Get this rectangle's size.
		constexpr Size<T> GetSize() const
		{ return { r - l, b - t }; }
		
		// Get this rectangle's center.
		constexpr Vec2<T> GetCenter() const
		{ return { (l + r) / T(2), (t + b) / T(2) }; }
		
		// Get this rectangle's top left corner.
		constexpr Vec2<T> GetMin() const
		{ return { l, t }; }
		
		// Get this rectangle's bottom right corner.
		constexpr Vec2<T> GetMax() const
		{ return { r, b }; }
	};
}


#endif // SMOL_CORE2D_RECT_DEFINED