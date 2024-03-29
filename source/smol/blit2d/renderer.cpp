#include "renderer.hpp"
#include <algorithm>


Smol::Blit2D::Renderer::Renderer() noexcept
{}


void Smol::Blit2D::Renderer::SetTarget(Bitmap& bitmap)
{
	target = &bitmap;
}


void Smol::Blit2D::Renderer::SetPalette(Palette& palette)
{
	this->palette = &palette;
}


void Smol::Blit2D::Renderer::SetColor(const Color& color)
{
	this->color = color;
}


void Smol::Blit2D::Renderer::DrawFill()
{
	auto ptr = &target->At({ 0, 0 });
	std::fill(ptr, ptr + (target->GetWidth() * target->GetHeight()), color);
}


void Smol::Blit2D::Renderer::DrawPoint(const Vec2I& pos)
{
	auto& point = target->At(pos);
	point = color;
}


void Smol::Blit2D::Renderer::DrawStride(const Vec2I& pos, int width)
{
	width = std::min<int>(width, target->GetWidth() - pos.x);
	std::fill(&target->At(pos), &target->At(pos) + width, color);
}


void Smol::Blit2D::Renderer::DrawRect(const RectI& dest)
{
	int width = std::min<int>(dest.size.x, target->GetWidth() - dest.pos.x);
	int height = std::min<int>(dest.size.y, target->GetHeight() - dest.pos.x);
	
	for (int y = dest.pos.y; y < height; y++)
	{
		auto ptr = &target->At({ dest.pos.x, y });
		std::fill(ptr, ptr + width, color);
	}
}


void Smol::Blit2D::Renderer::DrawBitmap(const Bitmap& bitmap, const Vec2I& pos, BlitOptions opts)
{
	if (pos.x < -bitmap.GetWidth() || pos.x > target->GetWidth() || pos.y < -bitmap.GetHeight() || pos.y > target->GetHeight())
	{ return; }
	
	auto x = std::max<int>(0, pos.x);
	auto y = std::max<int>(0, pos.y);
	auto w = std::min<int>({ bitmap.GetWidth(), pos.x + bitmap.GetWidth(), target->GetWidth() - pos.x });
	auto h = std::min<int>({ bitmap.GetHeight(), pos.y + bitmap.GetHeight(), target->GetHeight() - pos.y });
	auto offset = Vec2I((pos.x < 0) ? bitmap.GetWidth() - w : 0, (pos.y < 0) ? bitmap.GetHeight() - h : 0);
	
	for (int iy = 0; iy < h; iy++)
	{
		if (opts.flipy)
		{
			auto ptr = &bitmap.At({ offset.x, offset.y + h - 1 - iy });
			
			if (opts.flipx)
			{ std::reverse_copy(ptr, ptr + w, &target->At({ x, y + iy })); }
			else
			{ std::copy(ptr, ptr + w, &target->At({ x, y + iy })); }
		}
		else
		{
			auto ptr = &bitmap.At({ offset.x, offset.y + iy });
			
			if (opts.flipx)
			{ std::reverse_copy(ptr, ptr + w, &target->At({ x, y + iy })); }
			else
			{ std::copy(ptr, ptr + w, &target->At({ x, y + iy })); }
		}
	}
}