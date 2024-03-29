#ifndef SMOL_BLIT2D_COLOR_DEFINED
#define SMOL_BLIT2D_COLOR_DEFINED


namespace Smol::Blit2D
{
	enum class NamedColor
	{
		Empty,
		Black,
		White,
	};
	
	
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
		constexpr Color(NamedColor named_color) noexcept:
			r(0), g(0), b(0), a(0)
		{
			switch (named_color)
			{
				case NamedColor::Empty:
					*this = { 0, 0, 0, 0 };
					break;
			}
		}
		
		
		// Get an opaque copy of this color.
		constexpr Color GetOpaque() const
		{ return { r, g, b, 1.0f }; }
	};
}


#endif // SMOL_BLIT2D_COLOR_DEFINED