#include <array>
#include <string>
#include <random>
#include <smol/core2d.hpp>
#include <smol/blit2d.hpp>
#include "example_app.hpp"
using namespace Smol::Blit2D;
using namespace Smol;
namespace fs = std::filesystem;


// Asset directory containing card sprites
static const fs::path cards_dir = "assets/cards";

// Array of card suits
static const std::string card_suits[]
{ "hearts", "diamonds", "spades", "clubs" };

// Array of card ranks
static const std::string card_ranks[]
{ "2", "3", "4", "5", "6", "7", "8", "9", "10", "jack", "queen", "king", "ace" };


struct Card
{
	Vec2I pos;
	
	int card;
};


// Example 1 main app class
class App : public ExampleApp
{
	static constexpr SizeI CardSize = SizeI(13, 18);
	
	
	Renderer renderer;
	
	std::array<Bitmap, 52> card_bmps;
	
	Bitmap shadow;
	
	std::array<Card, 52> cards;
	
	int lift_index;
	
	Vec2I lift_pos;
	
	
 public:
	// Construct an app instance.
	App(HINSTANCE hInstance, const wchar_t* title, SizeI size, int scale) noexcept:
		ExampleApp(hInstance, title, { scale * size.w, scale * size.h }),
		renderer(size.w, size.h),
		lift_index(-1)
	{
		// Load card bitmaps
		int i = 0;
		
		for (const auto& suit : card_suits)
		{
			for (const auto& rank : card_ranks)
			{
				if (auto result = ExampleApp::LoadDDS(cards_dir/(rank + "-" + suit + ".dds")))
				{ card_bmps[i++] = std::move(*result); }
			}
		}
		
		// Randomly position our cards
		std::mt19937 mt;
		std::uniform_int_distribution x_dist{ 4, size.w - CardSize.w - 4 };
		std::uniform_int_distribution y_dist{ 4, size.h - CardSize.h - 4 };
		
		for (auto& card : cards)
		{ card.pos = { x_dist(mt), y_dist(mt) }; }
		
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
				for (int i = 0; i < cards.size(); i++)
				{
					if (RectI(cards[i].pos, CardSize).Contains(input.mouse_pos))
					{
						lift_index = i;
						lift_pos = input.mouse_pos - cards[i].pos;
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
		{ cards[lift_index].pos = input.mouse_pos - lift_pos; }
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
		
		for (int i = 0; i < cards.size(); i++)
		{
			if (lift_index != i)
			{ renderer.DrawBitmap(card_bmps[i], cards[i].pos); }
		}
		
		// Draw lifted card above the rest
		if (lift_index != -1)
		{
			renderer.DrawBitmap(card_bmps[lift_index], cards[lift_index].pos - Vec2I(1));
			renderer.DrawBitmap(shadow, cards[lift_index].pos);
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