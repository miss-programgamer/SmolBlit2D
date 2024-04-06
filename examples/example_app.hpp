#ifndef SMOL_BLIT2D_EXAMPLES_EXAMPLE_APP_DEFINED
#define SMOL_BLIT2D_EXAMPLES_EXAMPLE_APP_DEFINED


#include <optional>
#include <string_view>
#include <functional>
#include <filesystem>

#include <windows.h>
#include <tchar.h>
#include <d2d1.h>
#include <dwrite.h>
#undef LoadBitmap

#include <smol/blit2d/bitmap.hpp>

#include "input.hpp"


namespace Smol::Blit2D
{
	// 
	int ErrorMessageBox(const wchar_t* message, int error_code = 1);
	
	
	// 
	int StartMessageLoop();
	
	
	// 
	class ExampleApp
	{
		static inline std::wstring_view szMainWindowClass = L"MainWindow";
		
		
		HWND hWnd;
		
		IDWriteFactory* direct_write_factory;
		
		ID2D1Factory* direct_2d_factory;
		
		ID2D1HwndRenderTarget* rt;
		
		IDWriteTextFormat* text_format;
		
		ID2D1SolidColorBrush* brush;
		
		ID2D1Bitmap* bitmap_target;
		
		Input input;
		
		int time_passed;
		
		int time_target;
		
		int time_acc;
		
		ULONGLONG prev_time;
		
		
	 public:
		// Construct an example app.
		ExampleApp(HINSTANCE hInstance, const wchar_t* title, SizeI win_size) noexcept;
		
		// Show this example app's main window.
		void ShowMainWindow(int nCmdShow);
		
		// Whether this app has been successfully initialized.
		inline operator bool()
		{ return hWnd; }
		
	 protected:
		// Update function to be overriden by users.
		virtual void Update(const Input& input) = 0;
		
		// Draw function to be overriden by users.
		virtual const Bitmap& Draw() = 0;
		
		
	 private:
		static LRESULT WindowProc(_In_ HWND hWnd, _In_ UINT message, _In_ WPARAM wParam, _In_ LPARAM lParam);
		
		LRESULT HandleWindowMessage(_In_ HWND hWnd, _In_ UINT message, _In_ WPARAM wParam, _In_ LPARAM lParam);
		
		LRESULT HandleCreateMessage(_In_ HWND hWnd, _In_ UINT message, _In_ WPARAM wParam, _In_ LPARAM lParam);
		
		LRESULT HandleDestroyMessage(_In_ HWND hWnd, _In_ UINT message, _In_ WPARAM wParam, _In_ LPARAM lParam);
		
		LRESULT HandleSizeMessage(_In_ HWND hWnd, _In_ UINT message, _In_ WPARAM wParam, _In_ LPARAM lParam);
		
		LRESULT HandleKeyboardMessage(_In_ HWND hWnd, _In_ UINT message, _In_ WPARAM wParam, _In_ LPARAM lParam);
		
		LRESULT HandleMouseMessage(_In_ HWND hWnd, _In_ UINT message, _In_ WPARAM wParam, _In_ LPARAM lParam);
		
		LRESULT HandleTimerMessage(_In_ HWND hWnd, _In_ UINT message, _In_ WPARAM wParam, _In_ LPARAM lParam);
		
		LRESULT HandlePaintMessage(_In_ HWND hWnd, _In_ UINT message, _In_ WPARAM wParam, _In_ LPARAM lParam);
		
		// Write the contents of a given bitmap to the target bitmap.
		void UpdateBitmapTarget(const Bitmap& bitmap);
		
		
	 public:
		// Register example app window class.
		static bool RegisterWindowClass(HINSTANCE hInstance);
		
		// Load a bitmap from a .bmp file.
		static std::optional<Bitmap> LoadBMP(const std::filesystem::path& filename);
		
		// Load a bitmap from a .dds file.
		static std::optional<Bitmap> LoadDDS(const std::filesystem::path& filename);
	};
}


#endif // SMOL_BLIT2D_EXAMPLES_EXAMPLE_APP_DEFINED