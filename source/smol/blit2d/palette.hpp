#ifndef SMOL_BLIT2D_PALETTE_DEFINED
#define SMOL_BLIT2D_PALETTE_DEFINED


#include <initializer_list>
#include <cstdint>
#include <memory>
#include "color.hpp"


namespace Smol::Blit2D
{
	using palidx_t = uint8_t;
	
	
	// A list of at most 256 colors used to draw palette bitmaps.
	class Palette
	{
		static inline Color empty = ColorName::Empty;
		
		
		Color colors[255];
		
		
	 public:
		// Construct an empty palette.
		constexpr Palette() noexcept:
			colors{ ColorName::Empty }
		{}
		
		// Construct a palette from a given list.
		inline Palette(const std::initializer_list<Color>& colors)
		{ std::memcpy(this->colors, colors.begin(), std::min<size_t>(255, colors.size())); }
		
		
		// Index into this palette.
		inline Color& operator[](palidx_t index)
		{ return index ? colors[index - 1] : empty; }
		
		// Index into this const palette.
		inline const Color& operator[](palidx_t index) const
		{ return index ? colors[index - 1] : empty; }
	};
}


#endif // SMOL_BLIT2D_PALETTE_DEFINED