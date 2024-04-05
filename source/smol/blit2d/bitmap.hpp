#ifndef SMOL_BLIT2D_RAW_BITMAP_DEFINED
#define SMOL_BLIT2D_RAW_BITMAP_DEFINED


#include <cstdint>
#include <memory>

#include <smol/core2d.hpp>
#include "color.hpp"


namespace Smol::Blit2D
{
	using SizeU16 = Smol::Core2D::Size<uint16_t>;
	
	
	// A bitmap where pixels are full colors.
	class Bitmap
	{
		Color* colors;
		
		SizeU16 size;
		
		
	 public:
		// Construct an empty bitmap.
		constexpr Bitmap() noexcept:
			colors(nullptr),
			size{ 0, 0 }
		{}
		
		// Copy-construct a bitmap.
		Bitmap(const Bitmap& other);
		
		// Move-construct a bitmap
		Bitmap(Bitmap&& other) noexcept;
		
		// Copy-assign a bitmap.
		Bitmap& operator=(const Bitmap& other);
		
		// Move-assign a bitmap.
		Bitmap& operator=(Bitmap&& other);
		
		// Construct a bitmap of the given dimensions.
		Bitmap(uint16_t width, uint16_t height) noexcept;
		
		// Construct a bitmap of the given dimensions.
		Bitmap(SizeU16 size) noexcept;
		
		// Destruct this bitmap.
		~Bitmap();
		
		
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