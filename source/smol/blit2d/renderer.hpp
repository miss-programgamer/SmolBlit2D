#ifndef SMOL_BLIT2D_RENDERER_DEFINED
#define SMOL_BLIT2D_RENDERER_DEFINED


#include <cstdint>

#include "vec2.hpp"
#include "rect.hpp"
#include "color.hpp"
#include "palette.hpp"
#include "bitmap.hpp"


namespace Smol::Blit2D
{
	struct BlitOptions
	{
		bool flipx;
		bool flipy;
	};
	
	
	class Renderer
	{
		Bitmap* target;
		
		Palette* palette;
		
		Color color;
		
		
	 public:
		// Construct a renderer.
		Renderer() noexcept;
		
		
		// Set the target onto which we are drawing.
		void SetTarget(Bitmap& bitmap);
		
		// Set the palette used by this renderer.
		void SetPalette(Palette& palette);
		
		// Set the color used to tint bitmaps and primitives.
		void SetColor(const Color& color);
		
		
		// Fill the entire target with a solid color.
		void DrawFill();
		
		// Draw a single point of a solid color.
		void DrawPoint(const Vec2I& pos);
		
		// Draw a horizontal line of a solid color.
		void DrawStride(const Vec2I& pos, int width);
		
		// Draw a rectangle of a solid color.
		void DrawRect(const RectI& dest);
		
		// Draw a bitmap at the given position.
		void DrawBitmap(const Bitmap& bitmap, const Vec2I& pos, BlitOptions opts = {});
	};
}


#endif // SMOL_BLIT2D_RENDERER_DEFINED