#include <smol/core2d.hpp>
#include <smol/blit2d.hpp>
#include "example_app.hpp"
using namespace Smol::Blit2D;
using namespace Smol;


constexpr tileidx_t GrassTile = 1;
constexpr tileidx_t DirtTile = 2;
constexpr tileidx_t RockTile = 3;


// Example 1 main app class
class App : public ExampleApp
{
	Renderer renderer;
	
	Bitmap smiley;
	
	Bitmap player_bmp;
	
	Bitmap ground;
	
	Tileset ground_tileset;
	
	Tilemap tilemap;
	
	int time = 0;
	
	Vec2I player_pos;
	
	int player_dir;
	
	int player_face;
	
	
 public:
	// Construct an app instance.
	App(HINSTANCE hInstance, std::wstring_view title, int width, int height) noexcept:
		ExampleApp(hInstance, title, width, height),
		renderer(160, 120),
		ground_tileset({ 2, 2 }, { 8, 8 }),
		tilemap(160 / 8, 120 / 8),
		player_pos{ 160 / 2 - 4, 88 },
		player_dir(0),
		player_face(1)
	{
		// Load a smiley bitmap
		smiley = *ExampleApp::LoadDDS("assets/smiley.dds");
		
		// Load the player bitmap
		player_bmp = *ExampleApp::LoadDDS("assets/player.dds");
		
		// Load a ground tileset bitmap
		ground = *ExampleApp::LoadBMP("assets/ground.bmp");
		
		// Fill tilemap with stuff
		for (int i = 0; i < tilemap.GetWidth(); i++)
		{ tilemap.At({ i, tilemap.GetHeight() - 3 }) = GrassTile; }
		
		for (int i = 0; i < tilemap.GetWidth(); i++)
		{ tilemap.At({ i, tilemap.GetHeight() - 2 }) = DirtTile; }
		
		for (int i = 0; i < tilemap.GetWidth(); i++)
		{ tilemap.At({ i, tilemap.GetHeight() - 1 }) = DirtTile; }
	}
	
	
 protected:
	// Update function overriden by us.
	void Update(const Input& input) override
	{
		// Process our frame logic
		++time;
		
		if (Input::IsBtnPressed(input.pad_w_btn))
		{ player_dir = -1; }
		
		if (Input::IsBtnPressed(input.pad_e_btn))
		{ player_dir = 1; }
		
		if (Input::IsBtnDown(input.pad_n_btn))
		{ player_pos.y += -1; }
		
		if (Input::IsBtnDown(input.pad_s_btn))
		{ player_pos.y += 1; }
		
		if (Input::IsBtnReleased(input.pad_w_btn) || Input::IsBtnReleased(input.pad_e_btn))
		{ player_dir = Input::IsBtnDown(input.pad_e_btn) - Input::IsBtnDown(input.pad_w_btn); }
		
		if (player_dir != 0)
		{ player_face = player_dir; }
		
		player_pos.x += player_dir;
	}
	
	// Draw function overriden by us.
	const Bitmap& Draw() override
	{
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
		renderer.DrawBitmap(smiley, { time, 40 });
		renderer.DrawBitmap(smiley, { 45, 45 + (((time % 60) >= 30) ? 0 : 16) });
		
		// Draw player
		renderer.DrawBitmap(player_bmp, player_pos, { player_face == 1, false });
		
		// Return the result of drawing our frame
		return renderer.GetMainTarget();
	}
};


int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	// Register example app main window class
	if (!ExampleApp::RegisterWindowClass(hInstance))
	{ return ErrorMessageBox(L"Failed to register main window class", 1); }
	
	// Create example app
	App app(hInstance, L"Example 1", 640, 480);
	
	if (!app)
	{ return ErrorMessageBox(L"Failed to create main window", 1); }
	
	// Show main window
	app.ShowMainWindow(nCmdShow);
	
	// Main loop
	return StartMessageLoop();
}