#ifndef SMOL_BLIT2D_COLOR_DEFINED
#define SMOL_BLIT2D_COLOR_DEFINED


namespace Smol::Blit2D
{
	// Color names
	enum class ColorName
	{
		Empty,  Black,   White,
		Red,    Green,   Blue,
		Yellow, Magenta, Cyan,
	};
	
	
	// Color struct in RGBA128 float format.
	struct Color
	{
		float r;
		float g;
		float b;
		float a;
		
		
		// Construct a default color
		constexpr Color() noexcept:
			r(0), g(0), b(0), a(1)
		{}
		
		// Construct a color from its component values
		constexpr Color(float r, float g, float b, float a = 1.0f) noexcept:
			r(r), g(g), b(b), a(a)
		{}
		
		// Construct a grayscale color from an intensity and alpha
		constexpr Color(float v, float a = 1.0f) noexcept:
			r(v), g(v), b(v), a(a)
		{}
		
		// Construct a grayscale color from an intensity and alpha
		constexpr Color(ColorName name) noexcept:
			r(0), g(0), b(0), a(0)
		{
			switch (name)
			{
				case ColorName::Empty:
					*this = { 0, 0, 0, 0 };
					break;
				
				case ColorName::Black:
					*this = { 0, 0, 0, 1 };
					break;
				
				case ColorName::White:
					*this = { 1, 1, 1, 1 };
					break;
				
				case ColorName::Red:
					*this = { 1, 0, 0, 1 };
					break;
				
				case ColorName::Green:
					*this = { 0, 1, 0, 1 };
					break;
				
				case ColorName::Blue:
					*this = { 0, 0, 1, 1 };
					break;
				
				case ColorName::Yellow:
					*this = { 1, 1, 0, 1 };
					break;
				
				case ColorName::Magenta:
					*this = { 1, 0, 1, 1 };
					break;
				
				case ColorName::Cyan:
					*this = { 0, 1, 1, 1 };
					break;
			}
		}
		
		
		// Get a copy of this color with its alpha replaced.
		constexpr Color GetFaded(float alpha) const
		{ return { r, g, b, alpha }; }
		
		// Get an opaque copy of this color.
		constexpr Color GetOpaque() const
		{ return { r, g, b, 1.0f }; }
	};
}


#endif // SMOL_BLIT2D_COLOR_DEFINED