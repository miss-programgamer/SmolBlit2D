#include "renderer.hpp"
#include <stdexcept>
#include <algorithm>


Smol::Blit2D::Renderer::Renderer(int width, int height) noexcept:
	main_target(width, height),
	target(&main_target)
{}


Smol::Blit2D::Renderer::Renderer(SizeI size) noexcept:
	main_target(size.w, size.h),
	target(&main_target)
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
	int ox = (pos.x < 0) ? bmp_size.w - w : 0;
	int oy = (pos.y < 0) ? bmp_size.h - h : 0;
	
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
	int ox = (pos.x < 0) ? source.l + src_size.w - w : source.l;
	int oy = (pos.y < 0) ? source.t + src_size.h - h : source.t;
	
	DrawBitmap(bitmap, { x, y }, { w, h }, { ox, oy }, opts);
}


void Smol::Blit2D::Renderer::DrawBitmap(const Bitmap& bitmap, Vec2I pos, SizeI size, Vec2I offset, BlitOptions opts)
{
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
}