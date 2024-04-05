#include "bitmap.hpp"
#include <stdexcept>


Smol::Blit2D::Bitmap::Bitmap(const Bitmap& other):
	colors(new Color[other.size.w * other.size.h]),
	size(other.size)
{
	const unsigned int length = other.size.w * other.size.h;
	std::copy(other.colors, other.colors + length, colors);
}


Smol::Blit2D::Bitmap::Bitmap(Bitmap&& other) noexcept:
	colors(other.colors),
	size(other.size)
{
	other.colors = nullptr;
	other.size = { 0, 0 };
}


Smol::Blit2D::Bitmap& Smol::Blit2D::Bitmap::operator=(const Bitmap& other)
{
	// If colors array length is the same, just copy the memory
	if (GetArea() == other.GetArea())
	{
		std::copy(other.colors, other.colors + other.GetArea(), colors);
		return *this;
	}
	
	// Delete existing colors array
	if (colors != nullptr)
	{ delete[] colors; }
	
	// Make a copy of other bitmap's colors array
	colors = new Color[other.size.w, other.size.h];
	std::copy(other.colors, other.colors + other.GetArea(), colors);
	
	// Set size and return self
	size = other.size;
	return *this;
}


Smol::Blit2D::Bitmap& Smol::Blit2D::Bitmap::operator=(Bitmap&& other)
{
	// Delete existing colors array
	if (colors != nullptr)
	{ delete[] colors; }
	
	// Grab data from other bitmap
	colors = other.colors;
	size = other.size;
	
	// Reset other bitmap to be empty
	other.colors = nullptr;
	other.size = { 0, 0 };
	
	// Return self
	return *this;
}


Smol::Blit2D::Bitmap::Bitmap(uint16_t width, uint16_t height) noexcept:
	colors(new Color[width * height]),
	size{ width, height }
{}


Smol::Blit2D::Bitmap::Bitmap(SizeU16 size) noexcept:
	colors(new Color[size.w * size.h]),
	size(size)
{}


Smol::Blit2D::Bitmap::~Bitmap()
{
	if (colors != nullptr)
	{ delete[] colors; }
}


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