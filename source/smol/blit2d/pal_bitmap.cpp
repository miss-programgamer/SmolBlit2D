#include "pal_bitmap.hpp"
#include <stdexcept>


Smol::Blit2D::palidx_t& Smol::Blit2D::PalBitmap::At(int x, int y)
{
	if (x < 0 || x >= width || y < 0 || y >= height)
	{ throw std::out_of_range("position out of range of bitmap size"); }
	
	return indices[y * width + x];
}


const Smol::Blit2D::palidx_t& Smol::Blit2D::PalBitmap::At(int x, int y) const
{
	if (x < 0 || x >= width || y < 0 || y >= height)
	{ throw std::out_of_range("position out of range of bitmap size"); }
	
	return indices[y * width + x];
}