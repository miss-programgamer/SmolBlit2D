#ifndef SMOL_BLIT2D_RENDERER_DEFINED
#define SMOL_BLIT2D_RENDERER_DEFINED


#include <cstdint>

#include "vec2.hpp"
#include "rect.hpp"
#include "color.hpp"
#include "palette.hpp"
#include "pal_bitmap.hpp"
#include "bitmap.hpp"


namespace Smol::Blit2D
{
	// Option values relevant to drawing bitmaps.
	struct BlitOptions
	{
		bool flipx;
		bool flipy;
	};
	
	
	// Holds state & functionality relevant to drawing.
	class Renderer
	{
		Bitmap main_target;
		
		Bitmap* target;
		
		Palette* palette;
		
		Color color;
		
		
	 public:
		// Construct a renderer.
		Renderer(int width, int height) noexcept;
		
		// Construct a renderer.
		Renderer(SizeI size) noexcept;
		
		
		// Set the target onto which we are drawing.
		void SetTarget(Bitmap* bitmap);
		
		// Set the palette used by this renderer.
		void SetPalette(Palette* palette);
		
		// Set the color used to tint bitmaps and primitives.
		void SetColor(const Color& color);
		
		
		// Convert a palette bitmap to a bitmap using our palette.
		Bitmap ConvertToBitmap(const PalBitmap& bitmap);
		
		
		// Get a reference to the main target.
		inline const Bitmap& GetMainTarget() const
		{ return main_target; }
		
		
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
		
		// Draw a bitmap section at the given position.
		void DrawBitmap(const Bitmap& bitmap, const RectI& source, const Vec2I& pos, BlitOptions opts = {});
		
		
	 private:
		// Draw a bitmap section after its bounds have been computed.
		void DrawBitmap(const Bitmap& bitmap, Vec2I pos, SizeI size, Vec2I offset, BlitOptions opts);
	};
}


#endif // SMOL_BLIT2D_RENDERER_DEFINED