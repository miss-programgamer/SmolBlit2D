#ifndef SMOL_BLIT2D_TILESET_DEFINED
#define SMOL_BLIT2D_TILESET_DEFINED


#include <smol/core2d/vec2.hpp>
#include <smol/core2d/size.hpp>
#include <smol/core2d/rect.hpp>


namespace Smol::Blit2D
{
	using tileidx_t = int;
	
	
	// Contains the dimensions and spacing of tiles in a tileset.
	class Tileset
	{
		SizeI set_size;
		
		SizeI tile_size;
		
		SizeI spacing;
		
		RectI border;
		
		
	 public:
		// Construct an empty tileset.
		constexpr Tileset() noexcept:
			set_size{ 0, 0 },
			tile_size{ 0, 0 },
			spacing{ 0, 0 },
			border{ 0, 0, 0, 0 }
		{}
		
		// Construct a tileset from the size of the set, the tiles, and the spacing between tiles.
		constexpr Tileset(SizeI set_size, SizeI tile_size, SizeI spacing = { 0, 0 }, RectI border = { 0, 0, 0, 0 }) noexcept:
			set_size(set_size),
			tile_size(tile_size),
			spacing(spacing),
			border(border)
		{}
		
		
		// Get the source rectangle of a tile by its index.
		RectI GetSourceRect(tileidx_t index) const;
		
		
		constexpr SizeI GetSetSize() const
		{ return set_size; }
		
		constexpr SizeI GetTileSize() const
		{ return tile_size; }
		
		constexpr SizeI GetSpacing() const
		{ return spacing; }
		
		constexpr RectI GetBorder() const
		{ return border; }
	};
}


#endif // SMOL_BLIT2D_TILESET_DEFINED