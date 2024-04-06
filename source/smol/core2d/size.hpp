#ifndef SMOL_CORE2D_SIZE_DEFINED
#define SMOL_CORE2D_SIZE_DEFINED


namespace Smol::Core2D
{
	// A 2D size represented by two component values.
	template<typename T>
	struct Size
	{
		// Size width.
		T w;
		
		// Size height.
		T h;
		
		
		// Construct a size of zero.
		constexpr Size() noexcept(noexcept(T())):
			w(0), h(0)
		{}
		
		// Construct a size with the given value.
		constexpr explicit Size(T value) noexcept(noexcept(T())):
			w(value), h(value)
		{}
		
		// Construct a size with the given width and height.
		constexpr Size(T w, T h) noexcept(noexcept(T())):
			w(w), h(h)
		{}
		
		
		// Get the area of this size.
		constexpr T GetArea() const
		{ return w * h; }
	};
}


#endif // SMOL_CORE2D_SIZE_DEFINED