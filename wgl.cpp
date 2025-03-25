#include "gui.h"

void WGL_Gui::DestroyWGL() {

	wglMakeCurrent(nullptr, nullptr);
	ReleaseDC(window, glWindow);
	ZeroMemory(&pixelFormat, sizeof(pixelFormat));
}

bool WGL_Gui::CreateWGL() {

	if (window == nullptr) { return false; }
	glWindow = GetDC(window);

	ZeroMemory(&pixelFormat, sizeof(pixelFormat));
	pixelFormat.nSize = sizeof(pixelFormat);
	pixelFormat.nVersion = 1;
	pixelFormat.dwFlags = (PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER);
	pixelFormat.iPixelType = PFD_TYPE_RGBA;
	pixelFormat.cColorBits = 32;

	int format = ChoosePixelFormat(glWindow, &pixelFormat);
	if (format == 0) { return false; }
	
	if (!SetPixelFormat(glWindow, format, &pixelFormat)) { return false; }
	
	ReleaseDC(window, glWindow);
	glWindow = GetDC(window);
	
	if (hrc == nullptr) {
		hrc = wglCreateContext(glWindow);
	}

	wglMakeCurrent(glWindow, hrc);

	return true;
}

void WGL_Gui::ResetWGL(void (*updateFun)()) {

	DestroyWGL();
	if (updateFun != nullptr) { updateFun(); }
	CreateWGL();
}