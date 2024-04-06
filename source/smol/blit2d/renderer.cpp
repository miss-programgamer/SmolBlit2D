#include "renderer.hpp"
#include <stdexcept>
#include <algorithm>


Smol::Blit2D::Renderer::Renderer(int width, int height) noexcept:
	main_target(width, height),
	target(&main_target),
	mode(BlitMode::Replace)
{}


Smol::Blit2D::Renderer::Renderer(SizeI size) noexcept:
	main_target(size.w, size.h),
	target(&main_target),
	mode(BlitMode::Replace)
{}


void Smol::Blit2D::Renderer::SetTarget(Bitmap* bitmap)
{
	target = bitmap ? bitmap : &main_target;
}


void Smol::Blit2D::Renderer::SetPalette(Palette* palette)
{
	this->palette = palette;
}


void Smol::Blit2D::Renderer::SetColor(const Color& color)
{
	this->color = color;
}


void Smol::Blit2D::Renderer::SetBlitMode(BlitMode mode)
{
	this->mode = mode;
}


Smol::Blit2D::Bitmap Smol::Blit2D::Renderer::ConvertToBitmap(const PalBitmap& bitmap)
{
	if (palette == nullptr)
	{ throw std::logic_error("renderer palette was null"); }
	
	auto size = bitmap.GetSize();
	Bitmap result(size.w, size.h);
	
	for (int y = 0; y < size.h; y++)
	{
		for (int x = 0; x < size.w; x++)
		{ result.At({ x, y }) = (*palette)[bitmap.At({ x, y })]; }
	}
	
	return result;
}


void Smol::Blit2D::Renderer::DrawFill()
{
	auto ptr = &target->At({ 0, 0 });
	std::fill(ptr, ptr + target->GetArea(), color);
}


void Smol::Blit2D::Renderer::DrawPoint(const Vec2I& pos)
{
	auto tsize = target->GetSize();
	
	if (pos.x < 0 || pos.x >= tsize.w || pos.y < 0 || pos.y >= tsize.h)
	{ return; }
	
	auto& point = target->At(pos);
	point = color;
}


void Smol::Blit2D::Renderer::DrawStride(const Vec2I& pos, int width)
{
	auto tsize = target->GetSize();
	
	if (pos.x <= -width || pos.x >= tsize.w || pos.y < 0 || pos.y >= tsize.h)
	{ return; }
	
	int x = std::max<int>(0, pos.x);
	int w = std::min<int>(width, tsize.w - pos.x);
	
	auto ptr = &target->At({ x, pos.y });
	std::fill(ptr, ptr + w, color);
}


void Smol::Blit2D::Renderer::DrawRect(const RectI& dest)
{
	auto tsize = target->GetSize();
	auto dst_size = dest.GetSize();
	
	if (dest.r < 0 || dest.l >= tsize.w || dest.b < 0 || dest.t >= tsize.h)
	{ return; }
	
	int x = std::max<int>(0, dest.l);
	int y = std::max<int>(0, dest.t);
	int w = std::min<int>({ dst_size.w, dest.l + dst_size.w, tsize.w - dest.l });
	int h = std::min<int>({ dst_size.h, dest.t + dst_size.h, tsize.h - dest.t });
	
	for (int i = 0; i < h; i++)
	{
		auto ptr = &target->At({ x, y + i });
		std::fill(ptr, ptr + w, color);
	}
}


void Smol::Blit2D::Renderer::DrawBitmap(const Bitmap& bitmap, const Vec2I& pos, BlitOptions opts)
{
	auto tsize = target->GetSize();
	auto bmp_size = bitmap.GetSize();
	
	if (pos.x <= -bmp_size.w || pos.x >= tsize.w || pos.y <= -bmp_size.h || pos.y >= tsize.h)
	{ return; }
	
	auto x = std::max<int>(0, pos.x);
	auto y = std::max<int>(0, pos.y);
	auto w = std::min<int>({ bmp_size.w, pos.x + bmp_size.w, tsize.w - pos.x });
	auto h = std::min<int>({ bmp_size.h, pos.y + bmp_size.h, tsize.h - pos.y });
	int ox = (pos.x < 0 ^ opts.flipx) ? bmp_size.w - w : 0;
	int oy = (pos.y < 0 ^ opts.flipy) ? bmp_size.h - h : 0;
	
	DrawBitmap(bitmap, { x, y }, { w, h }, { ox, oy }, opts);
}


void Smol::Blit2D::Renderer::DrawBitmap(const Bitmap& bitmap, const RectI& source, const Vec2I& pos, BlitOptions opts)
{
	auto tsize = target->GetSize();
	auto src_size = source.GetSize();
	
	if (pos.x <= -src_size.w || pos.x >= tsize.w || pos.y <= -src_size.h || pos.y >= tsize.h)
	{ return; }
	
	int x = std::max<int>(0, pos.x);
	int y = std::max<int>(0, pos.y);
	int w = std::min<int>({ src_size.w, pos.x + src_size.w, tsize.w - pos.x });
	int h = std::min<int>({ src_size.h, pos.y + src_size.h, tsize.h - pos.y });
	int ox = (pos.x < 0 ^ opts.flipx) ? source.l + src_size.w - w : source.l;
	int oy = (pos.y < 0 ^ opts.flipy) ? source.t + src_size.h - h : source.t;
	
	DrawBitmap(bitmap, { x, y }, { w, h }, { ox, oy }, opts);
}


void Smol::Blit2D::Renderer::DrawTile(const Bitmap& bitmap, const Tileset& tileset, tileidx_t index, const Vec2I& pos, BlitOptions opts)
{
	auto tsize = target->GetSize();
	auto src_rect = tileset.GetSourceRect(index);
	auto src_size = src_rect.GetSize();
	
	if (pos.x <= -src_size.w || pos.x >= tsize.w || pos.y <= -src_size.h || pos.y >= tsize.h)
	{ return; }
	
	int x = std::max<int>(0, pos.x);
	int y = std::max<int>(0, pos.y);
	int w = std::min<int>({ src_size.w, pos.x + src_size.w, tsize.w - pos.x });
	int h = std::min<int>({ src_size.h, pos.y + src_size.h, tsize.h - pos.y });
	int ox = (pos.x < 0 ^ opts.flipx) ? src_rect.l + src_size.w - w : src_rect.l;
	int oy = (pos.y < 0 ^ opts.flipy) ? src_rect.t + src_size.h - h : src_rect.t;
	
	DrawBitmap(bitmap, { x, y }, { w, h }, { ox, oy }, opts);
}


void Smol::Blit2D::Renderer::DrawTilemap(const Bitmap& bitmap, const Tileset& tileset, const Tilemap& tilemap, const Vec2I& pos)
{
	auto tsize = target->GetSize();
	auto map_size = tilemap.GetSize();
	auto tile_size = tileset.GetTileSize();
	SizeI map_scale
	{
		map_size.w * tile_size.w,
		map_size.h * tile_size.h,
	};
	
	if (pos.x <= -map_scale.w || pos.x >= tsize.w || pos.y <= -map_scale.h || pos.y >= tsize.h)
	{ return; }
	
	for (int ty = 0; ty < map_size.h; ty++)
	{
		for (int tx = 0; tx < map_size.w; tx++)
		{
			auto tpos = Vec2I{ pos.x + tx * tile_size.w, pos.y + ty * tile_size.h };
			auto index = tilemap.At({ tx, ty });
			auto src_rect = tileset.GetSourceRect(index);
			auto src_size = src_rect.GetSize();
			
			if (index == 0 || tpos.x <= -src_size.w || tpos.x >= tsize.w || tpos.y <= -src_size.h || tpos.y >= tsize.h)
			{ continue; }
			
			int x = std::max<int>(0, tpos.x);
			int y = std::max<int>(0, tpos.y);
			int w = std::min<int>({ src_size.w, tpos.x + src_size.w, tsize.w - tpos.x });
			int h = std::min<int>({ src_size.h, tpos.y + src_size.h, tsize.h - tpos.y });
			int ox = (tpos.x < 0) ? src_rect.l + src_size.w - w : src_rect.l;
			int oy = (tpos.y < 0) ? src_rect.t + src_size.h - h : src_rect.t;
			
			DrawBitmap(bitmap, { x, y }, { w, h }, { ox, oy }, {});
		}
	}
}


void Smol::Blit2D::Renderer::DrawBitmap(const Bitmap& bitmap, Vec2I pos, SizeI size, Vec2I offset, BlitOptions opts)
{
	switch (mode)
	{
		case BlitMode::Replace:
			for (int i = 0; i < size.h; i++)
			{
				if (opts.flipy)
				{
					auto ptr = &bitmap.At({ offset.x, offset.y + size.h - 1 - i });
					
					if (opts.flipx)
					{ std::reverse_copy(ptr, ptr + size.w, &target->At({ pos.x, pos.y + i })); }
					else
					{ std::copy(ptr, ptr + size.w, &target->At({ pos.x, pos.y + i })); }
				}
				else
				{
					auto ptr = &bitmap.At({ offset.x, offset.y + i });
					
					if (opts.flipx)
					{ std::reverse_copy(ptr, ptr + size.w, &target->At({ pos.x, pos.y + i })); }
					else
					{ std::copy(ptr, ptr + size.w, &target->At({ pos.x, pos.y + i })); }
				}
			}
			break;
		
		case BlitMode::Blend:
			for (int iy = 0; iy < size.h; iy++)
			{
				for (int ix = 0; ix < size.w; ix++)
				{
					auto x = opts.flipx ? size.w - 1 - ix : ix;
					auto y = opts.flipy ? size.h - 1 - iy : iy;
					const auto& point = bitmap.At({ offset.x + x, offset.y + y });
					
					auto& base = target->At({ pos.x + ix, pos.y + iy });
					base = Color::Blend(base, point);
				}
			}
			break;
	}
}