#include "bitmap.hpp"
#include <stdexcept>


Smol::Blit2D::Color& Smol::Blit2D::Bitmap::At(const Vec2I& pos)
{
	if (pos.x < 0 || pos.x >= size.w || pos.y < 0 || pos.y >= size.h)
	{ throw std::out_of_range("position out of range of bitmap size"); }
	
	return colors[pos.y * size.w + pos.x];
}


const Smol::Blit2D::Color& Smol::Blit2D::Bitmap::At(const Vec2I& pos) const
{
	if (pos.x < 0 || pos.x >= size.w || pos.y < 0 || pos.y >= size.h)
	{ throw std::out_of_range("position out of range of bitmap size"); }
	
	return colors[pos.y * size.w + pos.x];
}