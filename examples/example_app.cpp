#include "example_app.hpp"


bool Smol::Blit2D::ExampleApp::RegisterMainWindowClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;
	
	wcex.cbSize         = sizeof(WNDCLASSEX);
	wcex.style          = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc    = WindowProc;
	wcex.cbClsExtra     = 0;
	wcex.cbWndExtra     = 0;
	wcex.hInstance      = hInstance;
	wcex.hIcon          = LoadIcon(wcex.hInstance, IDI_APPLICATION);
	wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName   = NULL;
	wcex.lpszClassName  = szMainWindowClass.data();
	wcex.hIconSm        = LoadIcon(wcex.hInstance, IDI_APPLICATION);
	
	return RegisterClassEx(&wcex);
}


int Smol::Blit2D::ExampleApp::RunApp()
{
	MSG msg;
	
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	
	return (int)msg.wParam;
}


Smol::Blit2D::ExampleApp::ExampleApp(HINSTANCE hInstance, std::wstring_view title) noexcept
{
	// Define constants for our window styles
	const DWORD style = WS_OVERLAPPEDWINDOW;
	const DWORD ex_style = WS_EX_OVERLAPPEDWINDOW;
	
	// Request a client rect instead of a window rect
	RECT rect{ 0, 0, 600, 400 };
	AdjustWindowRectEx(&rect, style, false, ex_style);
	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;
	
	// Create our window
	hWnd = CreateWindowEx(
		ex_style,
		szMainWindowClass.data(),
		title.data(),
		style,
		CW_USEDEFAULT, CW_USEDEFAULT,
		width, height,
		NULL,
		NULL,
		hInstance,
		this
	);
}


void Smol::Blit2D::ExampleApp::ShowMainWindow(int nCmdShow, const Bitmap& bitmap)
{
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	
	asdf->Release();
	rt->CreateBitmap(D2D1::SizeU(bitmap.GetWidth(), bitmap.GetHeight()), &bitmap.At({ 0, 0 }), sizeof(float) * 4 * bitmap.GetWidth(), D2D1::BitmapProperties(D2D1::PixelFormat(DXGI_FORMAT_R32G32B32A32_FLOAT, D2D1_ALPHA_MODE_IGNORE)), &asdf);
	RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE);
}


LRESULT Smol::Blit2D::ExampleApp::WindowProc(_In_ HWND hWnd, _In_ UINT message, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	switch (message)
	{
		case WM_CREATE:
			SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)(ExampleApp*)((CREATESTRUCT*)lParam)->lpCreateParams);
		default:
			return ((ExampleApp*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->HandleWindowMessage(hWnd, message, wParam, lParam);
	}
}


LRESULT Smol::Blit2D::ExampleApp::HandleWindowMessage(_In_ HWND hWnd, _In_ UINT message, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	switch (message)
	{
		case WM_CREATE:
			return HandleCreateMessage(hWnd, message, wParam, lParam);
		
		case WM_DESTROY:
			return HandleDestroyMessage(hWnd, message, wParam, lParam);
		
		case WM_SIZE:
			return HandleSizeMessage(hWnd, message, wParam, lParam);
		
		case WM_PAINT:
			return HandlePaintMessage(hWnd, message, wParam, lParam);
		
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
}


LRESULT Smol::Blit2D::ExampleApp::HandleCreateMessage(_In_ HWND hWnd, _In_ UINT message, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	RECT rect; GetWindowRect(hWnd, &rect);
	
	DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&direct_write_factory));
	D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &direct_2d_factory);
	
	direct_2d_factory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(), D2D1::HwndRenderTargetProperties(hWnd, D2D1::SizeU(rect.right - rect.left, rect.bottom - rect.top)), &rt);
	direct_write_factory->CreateTextFormat(_T("Arial"), NULL, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 24.0f, _T("en"), &text_format);
	rt->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &brush);
	
	if (auto result = rt->CreateBitmap(D2D1::SizeU(300, 200), D2D1::BitmapProperties(D2D1::PixelFormat(DXGI_FORMAT_R32G32B32A32_FLOAT, D2D1_ALPHA_MODE_IGNORE)), &asdf); !SUCCEEDED(result))
	{ MessageBox(NULL, L"Could not create bitmap", NULL, 0); }
	
	return 0;
}


LRESULT Smol::Blit2D::ExampleApp::HandleDestroyMessage(_In_ HWND hWnd, _In_ UINT message, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	PostQuitMessage(0);
	return 0;
}


LRESULT Smol::Blit2D::ExampleApp::HandleSizeMessage(_In_ HWND hWnd, _In_ UINT message, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	auto width = LOWORD(lParam);
	auto height = HIWORD(lParam);
	rt->Resize(D2D1::SizeU(width, height));
	return 0;
}


LRESULT Smol::Blit2D::ExampleApp::HandlePaintMessage(_In_ HWND hWnd, _In_ UINT message, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	// Begin drawing a frame
	PAINTSTRUCT ps;
	BeginPaint(hWnd, &ps);
	rt->BeginDraw();
	rt->Clear(D2D1::ColorF(D2D1::ColorF::Blue));
	
	// Draw a bitmap
	auto size = rt->GetSize();
	rt->DrawBitmap(asdf, D2D1::RectF(0, 0, size.width, size.height), 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);
	
	// Draw some text
	// std::wstring_view greeting = L"Hello, Windows desktop!";
	// auto size = rt->GetSize();
	// rt->DrawTextW(greeting.data(), (int)greeting.size(), text_format, D2D1::RectF(0, 0, size.width, size.height), brush);
	
	// End drawing our frame
	rt->EndDraw();
	EndPaint(hWnd, &ps);
	
	return 0;
}