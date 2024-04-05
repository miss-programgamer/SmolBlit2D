#ifndef SMOL_BLIT2D_TILEMAP_DEFINED
#define SMOL_BLIT2D_TILEMAP_DEFINED


#include <cstdint>
#include <memory>

#include <smol/core2d/vec2.hpp>
#include <smol/core2d/size.hpp>


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
		{ std::fill(tiles, tiles + (width * height), 0); }
		
		// Construct a tilemap of the given dimensions.
		inline Tilemap(Size<uint16_t> size) noexcept:
			tiles(new tileidx_t[size.w * size.h]),
			size(size)
		{ std::fill(tiles, tiles + (size.w * size.h), 0); }
		
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
		
		
		// Get this tilemap's width in number of tiles.
		constexpr uint16_t GetWidth() const
		{ return size.w; }
		
		// Get this tilemap's height in number of tiles.
		constexpr uint16_t GetHeight() const
		{ return size.h; }
		
		// Get this tilemap's size in number of tiles.
		constexpr SizeI GetSize() const
		{ return { size.w, size.h }; }
		
		// Get this tilemap's area in number of tiles.
		constexpr int GetArea() const
		{ return size.w * size.h; }
	};
}


#endif // SMOL_BLIT2D_TILEMAP_DEFINED