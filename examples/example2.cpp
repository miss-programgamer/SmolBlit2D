#include <array>
#include <smol/core2d.hpp>
#include <smol/blit2d.hpp>
#include "example_app.hpp"
using namespace Smol::Blit2D;
using namespace Smol;


// Example 1 main app class
class App : public ExampleApp
{
	static constexpr SizeI CardSize = SizeI(13, 18);
	
	
	Renderer renderer;
	
	std::array<Bitmap, 8> cards;
	
	Bitmap shadow;
	
	std::array<Vec2I, 8> cards_pos;
	
	int lift_index;
	
	Vec2I lift_pos;
	
	
 public:
	// Construct an app instance.
	App(HINSTANCE hInstance, const wchar_t* title, SizeI size, int scale) noexcept:
		ExampleApp(hInstance, title, { scale * size.w, scale * size.h }),
		renderer(size.w, size.h),
		cards_pos
		{
			Vec2I(23, 46),
			Vec2I(120, 32),
			Vec2I(96, 100),
			Vec2I(140, 60),
			Vec2I(150, 10),
			Vec2I(2, 90),
			Vec2I(50, 30),
			Vec2I(80, 5),
		},
		lift_index(-1)
	{
		// Load card bitmaps
		cards[0] = *ExampleApp::LoadDDS("assets/cards/ace-hearts.dds");
		cards[1] = *ExampleApp::LoadDDS("assets/cards/ace-diamonds.dds");
		cards[2] = *ExampleApp::LoadDDS("assets/cards/ace-spades.dds");
		cards[3] = *ExampleApp::LoadDDS("assets/cards/ace-clubs.dds");
		cards[4] = *ExampleApp::LoadDDS("assets/cards/2-hearts.dds");
		cards[5] = *ExampleApp::LoadDDS("assets/cards/2-diamonds.dds");
		cards[6] = *ExampleApp::LoadDDS("assets/cards/2-spades.dds");
		cards[7] = *ExampleApp::LoadDDS("assets/cards/2-clubs.dds");
		
		// Load the shadow bitmap
		shadow = *ExampleApp::LoadDDS("assets/shadow.dds");
	}
	
	
 protected:
	// Update function overriden by us.
	void Update(const Input& input) override
	{
		if (Input::IsBtnPressed(input.mouse_l_btn))
		{
			if (lift_index == -1)
			{
				for (int i = 0; i < cards_pos.size(); i++)
				{
					if (RectI(cards_pos[i], CardSize).Contains(input.mouse_pos))
					{
						lift_index = i;
						lift_pos = input.mouse_pos - cards_pos[i];
						break;
					}
				}
			}
			else
			{
				lift_index = -1;
			}
		}
		
		if (lift_index != -1)
		{ cards_pos[lift_index] = input.mouse_pos - lift_pos; }
	}
	
	// Draw function overriden by us.
	const Bitmap& Draw() override
	{
		// Init our frame
		renderer.SetTarget(nullptr);
		renderer.SetColor({ 0.1f, 0.4f, 0.15f });
		renderer.DrawFill();
		
		// Draw some cards to the main target
		renderer.SetBlitMode(BlitMode::Blend);
		
		for (int i = 0; i < cards_pos.size(); i++)
		{
			if (lift_index == i)
			{
				renderer.DrawBitmap(cards[i], cards_pos[i] - Vec2I(1));
				renderer.DrawBitmap(shadow, cards_pos[i]);
			}
			else
			{
				renderer.DrawBitmap(cards[i], cards_pos[i]);
			}
		}
		
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
	App app(hInstance, L"Example 2", { 160, 120 }, 6);
	
	if (!app)
	{ return ErrorMessageBox(L"Failed to create main window", 1); }
	
	// Show main window
	app.ShowMainWindow(nCmdShow);
	
	// Main loop
	return StartMessageLoop();
}