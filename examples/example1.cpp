#include <smol/blit2d.hpp>
using namespace Smol::Blit2D;
#include "example_app.hpp"


static int ErrorMessageBox(const wchar_t* message, int error_code = 1)
{
	MessageBoxW(NULL, message, NULL, MB_ICONERROR);
	return error_code;
}


constexpr tileidx_t GrassTile = 1;
constexpr tileidx_t DirtTile = 2;
constexpr tileidx_t RockTile = 3;


int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	// Register example app main window class
	if (!ExampleApp::RegisterMainWindowClass(hInstance))
	{ return ErrorMessageBox(L"Failed to register main window class", 1); }
	
	int x = 0;
	
	// Create blit2d renderer
	Renderer renderer(160, 120);
	
	// Load a smiley bitmap
	Bitmap smiley = *ExampleApp::LoadDDS("assets/smiley.dds");
	
	// Load a ground tileset bitmap
	Bitmap ground = *ExampleApp::LoadBMP("assets/ground.bmp");
	Tileset ground_tileset({ 2, 2 }, { 8, 8 });
	
	// Create a tilemap made of ground tiles
	Tilemap tilemap(160 / 8, 120 / 8);
	
	for (int i = 0; i < tilemap.GetWidth(); i++)
	{ tilemap.At({ i, tilemap.GetHeight() - 3 }) = GrassTile; }
	
	for (int i = 0; i < tilemap.GetWidth(); i++)
	{ tilemap.At({ i, tilemap.GetHeight() - 2 }) = DirtTile; }
	
	for (int i = 0; i < tilemap.GetWidth(); i++)
	{ tilemap.At({ i, tilemap.GetHeight() - 1 }) = DirtTile; }
	
	// Create example app
	ExampleApp example_app(hInstance, L"Example 1", 640, 480);
	
	if (!example_app)
	{ return ErrorMessageBox(L"Failed to create main window", 1); }
	
	// Show main window
	example_app.ShowMainWindow(nCmdShow, [&renderer, &smiley, &x, &ground, &ground_tileset, &tilemap]() -> const Bitmap*
	{
		// Process our frame logic
		x += 2;
		
		// Init our frame
		renderer.SetTarget(nullptr);
		renderer.SetColor({ 0.4f, 0.5f, 1 });
		renderer.DrawFill();
		
		// Draw a tilemap
		renderer.SetBlitMode(BlitMode::Replace);
		renderer.DrawTilemap(ground, ground_tileset, tilemap, { 0, 0 });
		
		// Draw some smileys to the main target
		renderer.SetBlitMode(BlitMode::Blend);
		renderer.DrawBitmap(smiley, { -2, -2 });
		renderer.DrawBitmap(smiley, { 65, 40 }, { false, false });
		renderer.DrawBitmap(smiley, { 85, 30 }, { true, false });
		renderer.DrawBitmap(smiley, { 90, 50 }, { true, true });
		renderer.DrawBitmap(smiley, { 150 - 6, 100 - 6 });
		
		// Draw our smiley fragmented into multiple parts
		renderer.DrawBitmap(smiley, { 0, 0, 4, 4 }, { 70, 60 });
		renderer.DrawBitmap(smiley, { 4, 0, 8, 4 }, { 75, 60 });
		renderer.DrawBitmap(smiley, { 4, 4, 8, 8 }, { 75, 65 });
		renderer.DrawBitmap(smiley, { 0, 4, 4, 8 }, { 70, 65 });
		
		// Draw a moving smiley
		renderer.DrawBitmap(smiley, { x, 40 });
		renderer.DrawBitmap(smiley, { 45, 45 + (((x % 60) >= 30) ? 0 : 16) });
		
		// Return the result of drawing our frame
		return &renderer.GetMainTarget();
	});
	
	// Main loop
	return ExampleApp::RunApp();
}