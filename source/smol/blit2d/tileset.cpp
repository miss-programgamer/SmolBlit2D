#include "tileset.hpp"
#include <algorithm>


Smol::Blit2D::RectI Smol::Blit2D::Tileset::GetSourceRect(tileidx_t index) const
{
	const int x = index % set_size.w;
	const int y = index / set_size.w;
	
	Vec2I pos
	{
		border.l + x * tile_size.w + std::max(0, x - 1) * spacing.w,
		border.t + y * tile_size.h + std::max(0, y - 1) * spacing.h,
	};
	
	return RectI(pos, tile_size);
}