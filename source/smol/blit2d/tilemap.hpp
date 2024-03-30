#ifndef SMOL_BLIT2D_TILEMAP_DEFINED
#define SMOL_BLIT2D_TILEMAP_DEFINED


#include <cstdint>
#include "vec2.hpp"
#include "size.hpp"


namespace Smol::Blit2D
{
	using tileidx_t = int;
	
	
	// Contains a grid of tile indices.
	class Tilemap
	{
		tileidx_t* tiles;
		
		Size<uint16_t> size;
		
		
	 public:
		// Construct an empty tilemap.
		constexpr Tilemap() noexcept:
			tiles(nullptr),
			size{ 0, 0 }
		{}
		
		// Construct a tilemap of the given dimensions.
		inline Tilemap(uint16_t width, uint16_t height) noexcept:
			tiles(new tileidx_t[width * height]),
			size{ width, height }
		{}
		
		// Construct a tilemap of the given dimensions.
		inline Tilemap(Size<uint16_t> size) noexcept:
			tiles(new tileidx_t[size.w * size.h]),
			size(size)
		{}
		
		// Destruct this tilemap.
		inline ~Tilemap()
		{
			if (tiles != nullptr)
			{ delete[] tiles; }
		}
		
		
		// Get a tile index by reference at the given point.
		tileidx_t& At(const Vec2I& pos);
		
		// Get a tile index by const reference at the given point.
		const tileidx_t& At(const Vec2I& pos) const;
	};
}


#endif // SMOL_BLIT2D_TILEMAP_DEFINED