#ifndef SMOL_BLIT2D_RAW_BITMAP_DEFINED
#define SMOL_BLIT2D_RAW_BITMAP_DEFINED


#include <cstdint>

#include "vec2.hpp"
#include "size.hpp"
#include "color.hpp"


namespace Smol::Blit2D
{
	// A bitmap where pixels are full colors.
	class Bitmap
	{
		Color* colors;
		
		Size<uint16_t> size;
		
		
	 public:
		// Construct an empty bitmap.
		constexpr Bitmap() noexcept:
			colors(nullptr),
			size{ 0, 0 }
		{}
		
		// Construct a bitmap of the given dimensions.
		inline Bitmap(uint16_t width, uint16_t height) noexcept:
			colors(new Color[width * height]),
			size{ width, height }
		{}
		
		// Construct a bitmap of the given dimensions.
		inline Bitmap(Size<uint16_t> size) noexcept:
			colors(new Color[size.w * size.h]),
			size(size)
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
		
		
		// Get this bitmap's width.
		constexpr uint16_t GetWidth() const
		{ return size.w; }
		
		// Get this bitmap's height.
		constexpr uint16_t GetHeight() const
		{ return size.h; }
		
		// Get this bitmap's size.
		constexpr SizeI GetSize() const
		{ return { size.w, size.h }; }
		
		// Get this bitmap's area.
		constexpr int GetArea() const
		{ return size.w * size.h; }
		
		// Check whether this bitmap contains anything.
		constexpr operator bool() const
		{ return colors != nullptr; }
	};
}


#endif // SMOL_BLIT2D_RAW_BITMAP_DEFINED