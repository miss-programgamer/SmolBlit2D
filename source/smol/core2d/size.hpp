#ifndef SMOL_CORE2D_SIZE_DEFINED
#define SMOL_CORE2D_SIZE_DEFINED


namespace Smol
{
	// A 2D size represented by two component values.
	template<typename T>
	struct Size
	{
		// Size width.
		T w;
		
		// Size height.
		T h;
		
		
		// Get the area of this size.
		constexpr T GetArea() const
		{ return w * h; }
	};
	
	
	using SizeI = Size<int>;
	using SizeF = Size<float>;
}


#endif // SMOL_CORE2D_SIZE_DEFINED