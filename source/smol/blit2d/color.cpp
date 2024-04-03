#include "color.hpp"


Smol::Blit2D::Color Smol::Blit2D::Color::Blend(const Color& other, const Color& base)
{
	const float a = base.a + other.a * (1.0f - base.a);
	float r = base.r * base.a + other.r * other.a * (1.0f - base.a) / a;
	float g = base.g * base.a + other.g * other.a * (1.0f - base.a) / a;
	float b = base.b * base.a + other.b * other.a * (1.0f - base.a) / a;
	return Color{ r, g, b, a };
}