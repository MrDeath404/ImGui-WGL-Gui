#include "gui.h"

void WGL_Gui::DestroyExWindow() {

	if (window != nullptr) {

		CloseWindow(window);
		DestroyWindow(window);
	}

	UnregisterClassW(wnd.lpszClassName, wnd.hInstance);
	ZeroMemory(&wnd, sizeof(wnd));
}

bool WGL_Gui::CreateExWindow(wstring name, WNDPROC wndProc, Vec2 size, Vec2 pos) {

	this->size = size;

	int bufferSize = WideCharToMultiByte(CP_UTF8, 0, name.c_str(), -1, NULL, 0, NULL, NULL);
	this->name = string(bufferSize, '\0');
	WideCharToMultiByte(CP_UTF8, 0, name.c_str(), -1, &this->name[0], bufferSize, NULL, NULL);

	ZeroMemory(&wnd, sizeof(wnd));
	wnd.cbSize = sizeof(wnd);
	wnd.style = 0U;
	wnd.lpfnWndProc = wndProc;
	wnd.cbClsExtra = 0;
	wnd.cbWndExtra = 0;
	wnd.hInstance = GetModuleHandle(NULL);
	wnd.hIcon = nullptr;
	wnd.hCursor = nullptr;
	wnd.hbrBackground = nullptr;
	wnd.lpszMenuName = nullptr;
	wnd.lpszClassName = (name + L"WNDClassExW").c_str();
	wnd.hIconSm = nullptr;

	RegisterClassExW(&wnd);

	window = CreateWindowExW(
		initExStyle, 
		wnd.lpszClassName, name.c_str(), 
		initStyle, 
		pos.x, pos.y, size.x, size.y, 
		nullptr, nullptr, 
		wnd.hInstance, nullptr
	);

	if (window == nullptr) { return false; }
}