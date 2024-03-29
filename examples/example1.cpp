#include <smol/blit2d.hpp>
#include "example_app.hpp"
using namespace Smol::Blit2D;


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
	
	// Create blit2d renderer
	Renderer renderer;
	
	Bitmap smiley(8, 8);
	renderer.SetTarget(smiley);
	renderer.SetColor({ 1, 1, 1, 1 });
	renderer.DrawFill();
	renderer.SetColor({ 0.5f, 0.5f, 0.5f, 1 });
	renderer.DrawPoint({ 1, 3 });
	renderer.DrawPoint({ 2, 3 });
	renderer.DrawPoint({ 5, 2 });
	renderer.DrawPoint({ 5, 3 });
	renderer.DrawStride({ 2, 6 }, 4);
	renderer.DrawPoint({ 1, 5 });
	renderer.DrawPoint({ 6, 5 });
	
	Bitmap target(150, 100);
	renderer.SetTarget(target);
	renderer.SetColor({ 0, 0, 0 });
	renderer.DrawFill();
	renderer.DrawBitmap(smiley, { -2, -2 });
	renderer.DrawBitmap(smiley, { 65, 40 }, { false, false });
	renderer.DrawBitmap(smiley, { 85, 30 }, { true, false });
	renderer.DrawBitmap(smiley, { 90, 50 }, { true, true });
	renderer.DrawBitmap(smiley, { 150 - 6, 100 - 6 });
	
	// Create example app
	ExampleApp example_app(hInstance, L"Example 1");
	
	if (!example_app)
	{ return ErrorMessageBox(L"Failed to create main window", 1); }
	
	// Show main window
	example_app.ShowMainWindow(nCmdShow, target);
	
	// Main loop
	return ExampleApp::RunApp();
}