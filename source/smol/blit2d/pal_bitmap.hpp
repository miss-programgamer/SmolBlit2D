#ifndef SMOL_BLIT2D_PAL_BITMAP_DEFINED
#define SMOL_BLIT2D_PAL_BITMAP_DEFINED


#include "vec2.hpp"
#include "size.hpp"
#include "palette.hpp"


namespace Smol::Blit2D
{
	// A bitmap where pixels are indices into a palette.
	class PalBitmap
	{
		palidx_t* indices;
		
		Size<uint16_t> size;
		
		
	 public:
		// Construct an empty bitmap.
		constexpr PalBitmap() noexcept:
			indices(nullptr),
			size{ 0, 0 }
		{}
		
		// Construct a bitmap of the given dimensions.
		inline PalBitmap(uint16_t width, uint16_t height) noexcept:
			indices(new palidx_t[width * height]),
			size{ width, height }
		{}
		
		// Construct a bitmap of the given dimensions.
		inline PalBitmap(Size<uint16_t> size) noexcept:
			indices(new palidx_t[size.w * size.h]),
			size(size)
		{}
		
		// Destruct this bitmap.
		inline ~PalBitmap()
		{
			if (indices != nullptr)
			{ delete[] indices; }
		}
		
		
		palidx_t& At(Vec2I pos);
		
		const palidx_t& At(Vec2I pos) const;
		
		
		constexpr int GetWidth() const
		{ return size.w; }
		
		constexpr int GetHeight() const
		{ return size.h; }
		
		constexpr SizeI GetSize() const
		{ return { size.w, size.h }; }
		
		constexpr int GetArea() const
		{ return size.w * size.h; }
		
		constexpr operator bool() const
		{ return indices != nullptr; }
	};
}


#endif // SMOL_BLIT2D_PAL_BITMAP_DEFINED