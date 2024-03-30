#include <smol/blit2d.hpp>
using namespace Smol::Blit2D;
#include "example_app.hpp"


static int ErrorMessageBox(const wchar_t* message, int error_code = 1)
{
	MessageBoxW(NULL, message, NULL, MB_ICONERROR);
	return error_code;
}


int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	// Register example app main window class
	if (!ExampleApp::RegisterMainWindowClass(hInstance))
	{ return ErrorMessageBox(L"Failed to register main window class", 1); }
	
	int x = 0;
	
	// Create blit2d renderer
	Renderer renderer(150, 100);
	
	// Load a smiley bitmap
	Bitmap smiley = *ExampleApp::LoadBitmap("assets/smiley.bmp");
	
	// Load a ground tileset bitmap
	Bitmap ground = *ExampleApp::LoadBitmap("assets/ground.bmp");
	Tileset ground_tileset({ 2, 2 }, { 8, 8 });
	
	// Create example app
	ExampleApp example_app(hInstance, L"Example 1");
	
	if (!example_app)
	{ return ErrorMessageBox(L"Failed to create main window", 1); }
	
	// Show main window
	example_app.ShowMainWindow(nCmdShow, [&renderer, &smiley, &x, &ground, &ground_tileset]() -> const Bitmap*
	{
		// Process our frame logic
		x += 2;
		
		// Init our frame
		renderer.SetTarget(nullptr);
		renderer.SetColor({ 0.4f, 0.5f, 1 });
		renderer.DrawFill();
		
		// Draw some things to the main target
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
		
		// Draw some tiles
		renderer.DrawTile(ground, ground_tileset, 1, { 24, 24 });
		renderer.DrawTile(ground, ground_tileset, 1, { 32, 24 });
		renderer.DrawTile(ground, ground_tileset, 1, { 40, 24 });
		renderer.DrawTile(ground, ground_tileset, 2, { 24, 32 });
		renderer.DrawTile(ground, ground_tileset, 2, { 32, 32 });
		renderer.DrawTile(ground, ground_tileset, 2, { 40, 32 });
		
		// Draw a moving smiley
		renderer.DrawBitmap(smiley, { x, 40 });
		
		// Return the result of drawing our frame
		return &renderer.GetMainTarget();
	});
	
	// Main loop
	return ExampleApp::RunApp();
}