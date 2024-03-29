#ifndef SMOL_BLIT2D_PAL_BITMAP_DEFINED
#define SMOL_BLIT2D_PAL_BITMAP_DEFINED


#include "palette.hpp"


namespace Smol::Blit2D
{
	// A bitmap where pixels are indices into a palette.
	class PalBitmap
	{
		palidx_t* indices;
		
		uint16_t width;
		
		uint16_t height;
		
		
		public:
		// Construct an empty bitmap.
		constexpr PalBitmap() noexcept:
			indices(nullptr),
			width(0), height(0)
		{}
		
		// Construct a bitmap of the given dimensions.
		inline PalBitmap(uint16_t width, uint16_t height) noexcept:
			indices(new palidx_t[width * height]),
			width(width), height(height)
		{}
		
		// Destruct this bitmap.
		inline ~PalBitmap()
		{
			if (indices != nullptr)
			{ delete[] indices; }
		}
		
		
		palidx_t& At(int x, int y);
		
		const palidx_t& At(int x, int y) const;
	};
}


#endif // SMOL_BLIT2D_PAL_BITMAP_DEFINED