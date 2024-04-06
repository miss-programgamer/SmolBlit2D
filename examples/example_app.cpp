#include "example_app.hpp"
#include <array>
#include <fstream>


#define UPDATE_KEY(btn, state) btn = (btn & 0b10) | ((state & 0x8000) ? 0b01 : 0b00)
#define UPDATE_BTN(btn, state) btn = (btn & 0b10) | ((state) ? 0b01 : 0b00);


int Smol::Blit2D::ErrorMessageBox(const wchar_t* message, int error_code)
{
	MessageBoxW(NULL, message, NULL, MB_ICONERROR);
	return error_code;
}


int Smol::Blit2D::StartMessageLoop()
{
	MSG msg;
	
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	
	return (int)msg.wParam;
}


bool Smol::Blit2D::ExampleApp::RegisterWindowClass(HINSTANCE hInstance)
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


Smol::Blit2D::ExampleApp::ExampleApp(HINSTANCE hInstance, std::wstring_view title, int width, int height) noexcept:
	input{},
	time_target(16)
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


void Smol::Blit2D::ExampleApp::ShowMainWindow(int nCmdShow)
{
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	
	UpdateBitmapTarget(Draw());
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
		
		case WM_KEYDOWN:
		case WM_KEYUP:
			return HandleKeyboardMessage(hWnd, message, wParam, lParam);
		
		case WM_MOUSEMOVE:
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_MBUTTONDOWN:
		case WM_MBUTTONUP:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
			return HandleMouseMessage(hWnd, message, wParam, lParam);
		
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
	prev_time = GetTickCount64();
	
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


LRESULT Smol::Blit2D::ExampleApp::HandleKeyboardMessage(_In_ HWND hWnd, _In_ UINT message, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	UPDATE_KEY(input.face_n_btn, GetKeyState('S'));
	UPDATE_KEY(input.face_s_btn, GetKeyState('Z'));
	UPDATE_KEY(input.face_w_btn, GetKeyState('A'));
	UPDATE_KEY(input.face_e_btn, GetKeyState('X'));
	UPDATE_KEY(input.shld_l_btn, GetKeyState('D'));
	UPDATE_KEY(input.shld_r_btn, GetKeyState('C'));
	UPDATE_KEY(input.pad_n_btn,  GetKeyState(VK_UP));
	UPDATE_KEY(input.pad_s_btn,  GetKeyState(VK_DOWN));
	UPDATE_KEY(input.pad_w_btn,  GetKeyState(VK_LEFT));
	UPDATE_KEY(input.pad_e_btn,  GetKeyState(VK_RIGHT));
	return 0;
}


LRESULT Smol::Blit2D::ExampleApp::HandleMouseMessage(_In_ HWND hWnd, _In_ UINT message, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	UPDATE_BTN(input.mouse_l_btn, wParam & MK_LBUTTON);
	UPDATE_BTN(input.mouse_m_btn, wParam & MK_MBUTTON);
	UPDATE_BTN(input.mouse_r_btn, wParam & MK_RBUTTON);
	input.mouse_pos = { LOWORD(lParam) / 4, HIWORD(lParam) / 4 };
	return 0;
}


LRESULT Smol::Blit2D::ExampleApp::HandleTimerMessage(_In_ HWND hWnd, _In_ UINT message, _In_ WPARAM wParam, _In_ LPARAM lParam)
{ return 0; }


LRESULT Smol::Blit2D::ExampleApp::HandlePaintMessage(_In_ HWND hWnd, _In_ UINT message, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	// Update game state if needed
	auto time = GetTickCount64();
	auto delta = time - prev_time;
	int next_target = time_target;
	
	time_passed += delta;
	
	if ((time_passed / time_target) > 0)
	{
		UpdateBitmapTarget((Update(input), Draw()));
		input.Cycle();
		
		time_acc += 2;
		next_target = ((time_acc / 3) > 0) ? 17 : 16;
		time_acc %= 3;
	}
	
	time_passed %= time_target;
	time_target = next_target;
	prev_time = time;
	
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
	
	// Invalidate entire window, ensuring a repaint next frame
	RedrawWindow(hWnd, NULL, NULL, RDW_INTERNALPAINT);
	
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


std::optional<Smol::Blit2D::Bitmap> Smol::Blit2D::ExampleApp::LoadBMP(const char* filename)
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
	
	// find size of bitmap header (should be 40)
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
			
			if (color.r == 1.0f && color.g == 0.0f && color.b == 1.0f)
			{ color.a = 0.0f; }
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


std::optional<Smol::Blit2D::Bitmap> Smol::Blit2D::ExampleApp::LoadDDS(const char* filename)
{
	// NOTE: this function has like zero error handling and is really basic
	//       don't actually use this for anything important y'all
	
	// magic value indicating a DDS file (literally just "DDS\0" in hex)
	const uint32_t DDS_SENTINEL_VALUE = 0x20534444;
	
	// open bitmap file
	std::ifstream file(filename, std::ios::in|std::ios::binary);
	
	// ensure file is of the right format
	if (uint32_t sentinel; !file.read((char*)&sentinel, sizeof(sentinel)) || sentinel != DDS_SENTINEL_VALUE)
	{ return std::nullopt; }
	
	// ensure size of header is 124 bytes
	if (uint32_t header_size; !file.read((char*)&header_size, sizeof(header_size)) || header_size != 124)
	{ return std::nullopt; }
	
	// read header flags
	uint32_t header_flags;
	
	if (!file.read((char*)&header_flags, sizeof(header_flags)))
	{ return std::nullopt; }
	
	uint32_t header_height;
	uint32_t header_width;
	
	if (!file.read((char*)&header_height, sizeof(header_height)))
	{ return std::nullopt; }
	
	if (!file.read((char*)&header_width, sizeof(header_width)))
	{ return std::nullopt; }
	
	uint32_t header_pitch;
	
	if (!file.read((char*)&header_pitch, sizeof(header_pitch)))
	{ return std::nullopt; }
	
	uint32_t header_depth;
	
	if (!file.read((char*)&header_depth, sizeof(header_depth)))
	{ return std::nullopt; }
	
	uint32_t header_mipmap_levels;
	
	if (!file.read((char*)&header_mipmap_levels, sizeof(header_mipmap_levels)))
	{ return std::nullopt; }
	
	// skip reserved/unused space
	if (!file.seekg(std::streamoff(11 * sizeof(uint32_t)), std::ios_base::cur))
	{ return std::nullopt; }
	
	uint32_t pixel_size;
	
	if (!file.read((char*)&pixel_size, sizeof(pixel_size)) || pixel_size != 32)
	{ return std::nullopt; }
	
	uint32_t pixel_flags;
	
	if (!file.read((char*)&pixel_flags, sizeof(pixel_flags)))
	{ return std::nullopt; }
	
	std::array<char, 4> pixel_4charcode;
	
	if (!file.read(pixel_4charcode.data(), pixel_4charcode.size()))
	{ return std::nullopt; }
	
	uint32_t pixel_bitcount;
	
	if (!file.read((char*)&pixel_bitcount, sizeof(pixel_bitcount)))
	{ return std::nullopt; }
	
	uint32_t pixel_rmask;
	
	if (!file.read((char*)&pixel_rmask, sizeof(pixel_rmask)))
	{ return std::nullopt; }
	
	uint32_t pixel_gmask;
	
	if (!file.read((char*)&pixel_gmask, sizeof(pixel_gmask)))
	{ return std::nullopt; }
	
	uint32_t pixel_bmask;
	
	if (!file.read((char*)&pixel_bmask, sizeof(pixel_bmask)))
	{ return std::nullopt; }
	
	uint32_t pixel_amask;
	
	if (!file.read((char*)&pixel_amask, sizeof(pixel_amask)))
	{ return std::nullopt; }
	
	uint32_t header_caps1;
	
	if (!file.read((char*)&header_caps1, sizeof(header_caps1)))
	{ return std::nullopt; }
	
	uint32_t header_caps2;
	
	if (!file.read((char*)&header_caps2, sizeof(header_caps2)))
	{ return std::nullopt; }
	
	// skip some more reserved/unused space
	if (!file.seekg(std::streamoff(3 * sizeof(uint32_t)), std::ios_base::cur))
	{ return std::nullopt; }
	
	Bitmap result(header_width, header_height);
	
	for (int y = 0; y < header_height; y++)
	{
		for (int x = 0; x < header_width; x++)
		{
			std::array<unsigned char, 4> pixel_data;
			file.read((char*)pixel_data.data(), pixel_data.size());
			result.At({ x, y }) = Color
			{
				pixel_data[0] / 255.0f,
				pixel_data[1] / 255.0f,
				pixel_data[2] / 255.0f,
				pixel_data[3] / 255.0f,
			};
		}
	}
	
	return result;
}