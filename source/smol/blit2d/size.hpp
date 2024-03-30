#ifndef SMOL_BLIT2D_SIZE_DEFINED
#define SMOL_BLIT2D_SIZE_DEFINED


namespace Smol::Blit2D
{
	// A 2D size represented by two component values.
	template<typename T>
	struct Size
	{
		T w;
		T h;
		
		
		// Get the area of this size.
		constexpr T GetArea() const
		{ return w * h; }
	};
	
	
	using SizeI = Size<int>;
	using SizeF = Size<float>;
}


#endif // SMOL_BLIT2D_SIZE_DEFINED