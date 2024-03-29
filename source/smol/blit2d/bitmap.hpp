#ifndef SMOL_BLIT2D_RAW_BITMAP_DEFINED
#define SMOL_BLIT2D_RAW_BITMAP_DEFINED


#include <cstdint>

#include "vec2.hpp"
#include "color.hpp"


namespace Smol::Blit2D
{
	// A bitmap where pixels are full colors.
	class Bitmap
	{
		Color* colors;
		
		uint16_t width;
		
		uint16_t height;
		
		
	 public:
		// Construct an empty bitmap.
		constexpr Bitmap() noexcept:
			colors(nullptr),
			width(0), height(0)
		{}
		
		// Construct a bitmap of the given dimensions.
		inline Bitmap(uint16_t width, uint16_t height) noexcept:
			colors(new Color[width * height]),
			width(width), height(height)
		{}
		
		// Destruct this bitmap.
		inline ~Bitmap()
		{
			if (colors != nullptr)
			{ delete[] colors; }
		}
		
		
		// Get a pixel at the given point in this bitmap.
		Color& At(const Vec2I& pos);
		
		// Get a pixel at the given point in this const bitmap.
		const Color& At(const Vec2I& pos) const;
		
		
		constexpr uint16_t GetWidth() const
		{ return width; }
		
		constexpr uint16_t GetHeight() const
		{ return height; }
		
		constexpr Vec2I GetSize() const
		{ return { width, height }; }
		
		constexpr operator bool() const
		{ return colors != nullptr; }
	};
}


#endif // SMOL_BLIT2D_RAW_BITMAP_DEFINED