#ifndef SMOL_BLIT2D_EXAMPLE_APP_DEFINED
#define SMOL_BLIT2D_EXAMPLE_APP_DEFINED


#include <optional>
#include <string_view>
#include <functional>

#include <windows.h>
#include <tchar.h>
#include <d2d1.h>
#include <dwrite.h>
#undef LoadBitmap

#include <smol/blit2d/bitmap.hpp>


namespace Smol::Blit2D
{
	class ExampleApp
	{
		static inline std::wstring_view szMainWindowClass = L"MainWindow";
		
		
		HWND hWnd;
		
		IDWriteFactory* direct_write_factory;
		
		ID2D1Factory* direct_2d_factory;
		
		ID2D1HwndRenderTarget* rt;
		
		IDWriteTextFormat* text_format;
		
		ID2D1SolidColorBrush* brush;
		
		ID2D1Bitmap* asdf;
		
		std::function<const Bitmap*()> callback;
		
		
	 public:
		static bool RegisterMainWindowClass(HINSTANCE hInstance);
		
		static int RunApp();
		
		ExampleApp(HINSTANCE hInstance, std::wstring_view title) noexcept;
		
		void ShowMainWindow(int nCmdShow, std::function<const Bitmap*()> callback);
		
		static LRESULT WindowProc(_In_ HWND hWnd, _In_ UINT message, _In_ WPARAM wParam, _In_ LPARAM lParam);
		
		LRESULT HandleWindowMessage(_In_ HWND hWnd, _In_ UINT message, _In_ WPARAM wParam, _In_ LPARAM lParam);
		
		LRESULT HandleCreateMessage(_In_ HWND hWnd, _In_ UINT message, _In_ WPARAM wParam, _In_ LPARAM lParam);
		
		LRESULT HandleDestroyMessage(_In_ HWND hWnd, _In_ UINT message, _In_ WPARAM wParam, _In_ LPARAM lParam);
		
		LRESULT HandleSizeMessage(_In_ HWND hWnd, _In_ UINT message, _In_ WPARAM wParam, _In_ LPARAM lParam);
		
		LRESULT HandleTimerMessage(_In_ HWND hWnd, _In_ UINT message, _In_ WPARAM wParam, _In_ LPARAM lParam);
		
		LRESULT HandlePaintMessage(_In_ HWND hWnd, _In_ UINT message, _In_ WPARAM wParam, _In_ LPARAM lParam);
		
		inline operator bool()
		{ return hWnd; }
		
		
		static std::optional<Bitmap> LoadBitmap(const char* filename);
	};
}


#endif // SMOL_BLIT2D_EXAMPLE_APP_DEFINED