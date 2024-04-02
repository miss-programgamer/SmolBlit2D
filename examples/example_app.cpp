#include "example_app.hpp"
#include <array>
#include <fstream>


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


Smol::Blit2D::ExampleApp::ExampleApp(HINSTANCE hInstance, std::wstring_view title, int width, int height) noexcept
{
	// Define constants for our window styles
	const DWORD style = WS_OVERLAPPEDWINDOW;
	const DWORD ex_style = WS_EX_OVERLAPPEDWINDOW;
	
	// Request a client rect instead of a window rect
	RECT rect{ 0, 0, width, height };
	AdjustWindowRectEx(&rect, style, false, ex_style);
	width = rect.right - rect.left;
	height = rect.bottom - rect.top;
	
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


void Smol::Blit2D::ExampleApp::ShowMainWindow(int nCmdShow, std::function<const Bitmap*()> callback)
{
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	
	SetTimer(hWnd, 0, 1000 / 50, NULL);
	
	this->callback = std::move(callback);
	
	UpdateBitmapTarget(*this->callback());
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
		
		case WM_TIMER:
			return HandleTimerMessage(hWnd, message, wParam, lParam);
		
		case WM_PAINT:
			return HandlePaintMessage(hWnd, message, wParam, lParam);
		
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
}


LRESULT Smol::Blit2D::ExampleApp::HandleCreateMessage(_In_ HWND hWnd, _In_ UINT message, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	RECT rect; GetWindowRect(hWnd, &rect);
	auto size = D2D1::SizeU(rect.right - rect.left, rect.bottom - rect.top);
	
	DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&direct_write_factory));
	D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &direct_2d_factory);
	
	direct_2d_factory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(), D2D1::HwndRenderTargetProperties(hWnd, size), &rt);
	direct_write_factory->CreateTextFormat(_T("Arial"), NULL, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 24.0f, _T("en"), &text_format);
	rt->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &brush);
	
	if (auto result = rt->CreateBitmap(size, D2D1::BitmapProperties(D2D1::PixelFormat(DXGI_FORMAT_R32G32B32A32_FLOAT, D2D1_ALPHA_MODE_IGNORE)), &bitmap_target); !SUCCEEDED(result))
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


LRESULT Smol::Blit2D::ExampleApp::HandleTimerMessage(_In_ HWND hWnd, _In_ UINT message, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	UpdateBitmapTarget(*callback());
	RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE);
	return 0;
}


LRESULT Smol::Blit2D::ExampleApp::HandlePaintMessage(_In_ HWND hWnd, _In_ UINT message, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	// Begin drawing a frame
	PAINTSTRUCT ps;
	BeginPaint(hWnd, &ps);
	rt->BeginDraw();
	rt->Clear(D2D1::ColorF(D2D1::ColorF::Blue));
	
	// Draw our bitmap target bitmap
	auto size = rt->GetSize();
	rt->DrawBitmap(bitmap_target, D2D1::RectF(0, 0, size.width, size.height), 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);
	
	// End drawing our frame
	rt->EndDraw();
	EndPaint(hWnd, &ps);
	
	return 0;
}


void Smol::Blit2D::ExampleApp::UpdateBitmapTarget(const Bitmap& bitmap)
{
	auto target_size = bitmap_target->GetPixelSize();
	auto bitmap_size = D2D1::SizeU(bitmap.GetWidth(), bitmap.GetHeight());
	
	if (target_size == bitmap_size)
	{
		bitmap_target->CopyFromMemory(NULL, &bitmap.At({ 0, 0 }), sizeof(float) * 4 * bitmap.GetWidth());
	}
	else
	{
		bitmap_target->Release();
		auto props = D2D1::BitmapProperties(D2D1::PixelFormat(DXGI_FORMAT_R32G32B32A32_FLOAT, D2D1_ALPHA_MODE_IGNORE));
		rt->CreateBitmap(bitmap_size, &bitmap.At({ 0, 0 }), sizeof(float) * 4 * bitmap.GetWidth(), props, &bitmap_target);
	}
}


std::optional<Smol::Blit2D::Bitmap> Smol::Blit2D::ExampleApp::LoadBitmap(const char* filename)
{
	// open bitmap file
	std::ifstream file(filename, std::ios::binary);
	
	// load file header
	std::array<unsigned char, 14> file_header;
	
	for (auto& c : file_header)
	{ c = file.get(); }
	
	// find file size
	uint32_t file_size;
	memcpy(&file_size, &file_header[2], 4);
	
	// find pixel data offset
	uint32_t pixel_data_index;
	memcpy(&pixel_data_index, &file_header[10], 4);
	
	// load bitmap header (different from the file header)
	std::array<unsigned char, 40> bitmap_header;
	
	for (auto& c : bitmap_header)
	{ c = file.get(); }
	
	// fine size of bitmap header (should be 40)
	uint32_t bitmap_header_size;
	memcpy(&bitmap_header_size, &bitmap_header[0], 4);
	
	// find image size
	int32_t width, height, area;
	memcpy(&width, &bitmap_header[4], 4);
	memcpy(&height, &bitmap_header[8], 4);
	area = width * height;
	
	// find color depth
	int16_t bit_depth;
	memcpy(&bit_depth, &bitmap_header[14], 2);
	
	// find compression method
	uint32_t compression_method;
	memcpy(&compression_method, &bitmap_header[16], 4);
	
	// find palette size
	uint32_t palette_size;
	memcpy(&palette_size, &bitmap_header[32], 4);
	
	if (palette_size == 0)
	{ palette_size = 256; }
	
	if (bit_depth <= 8) // the image uses a color palette
	{
		// create and fill color palette
		Color* palette_colors = new Color[palette_size];
		
		for (uint32_t i = 0; i < palette_size; i++)
		{
			auto& color = palette_colors[i];
			color.b = (float)file.get() / 255.0f;
			color.g = (float)file.get() / 255.0f;
			color.r = (float)file.get() / 255.0f;
			color.a = (float)file.get() / 255.0f;
		}
		
		// create and fill bitmap
		Bitmap bitmap(width, height);
		auto colors = &bitmap.At({ 0, 0 });
		
		for (int i = 0; i < area; i++)
		{
			auto x = i % width;
			auto y = height - i / width - 1;
			
			if (unsigned char index = file.get(); index < palette_size)
			{ bitmap.At({ x, y }) = palette_colors[index]; }
		}
		
		// delete our color palette
		delete[] palette_colors;
		
		// return complete bitmap
		return bitmap;
	}
	else // the image uses colors directly
	{
		// return nothing for now
		return std::nullopt;
	}
}