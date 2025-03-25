#pragma once
#pragma comment(lib, "opengl32.lib")
#include <imgui.h>
#include <Windows.h>
#include <gl/GL.h>
#include <thread>
#include <string>

using namespace std;

struct Vec2 {
	
	int x, y;

	ImVec2 ToImVec2() {
		return ImVec2{ (float)(x), (float)(y) };
	}
};

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

inline LRESULT WINAPI DefaultWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam)) {
		return 1;
	}

	switch (msg) {
		case WM_DESTROY: {
			PostQuitMessage(0);
		} return 0;
	}
	return DefWindowProcW(hwnd, msg, wParam, lParam);
}

inline POINTS curPoints;
inline LRESULT WINAPI DefaultMovingWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam)) {
		return 1;
	}

	switch (msg) {
	case WM_DESTROY: {
		PostQuitMessage(0);
	} return 0;
	case WM_LBUTTONDOWN: {
		curPoints = MAKEPOINTS(lParam);
	} return 0;
	case WM_MOUSEMOVE: {
		if (wParam == MK_LBUTTON) {
			POINTS points = MAKEPOINTS(lParam);
			RECT rect;
			GetWindowRect(hwnd, &rect);

			RECT newRect = rect;
			newRect.left += points.x - curPoints.x;
			newRect.top += points.y - curPoints.y;

			if (curPoints.x >= 0 &&
				curPoints.x <= (rect.right - rect.left) &&
				curPoints.y >= 0 &&
				curPoints.y <= 21
				) {
				SetWindowPos(hwnd, HWND_TOPMOST, newRect.left, newRect.top, 0, 0, (SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOZORDER));
			}
		}
	} return 0;
	}
	return DefWindowProcW(hwnd, msg, wParam, lParam);
}

class WGL_Gui {
public:
	void DestroyExWindow();
	bool CreateExWindow(wstring name, WNDPROC wndProc, Vec2 size, Vec2 pos = { 0, 0 });

	void DestroyWGL();
	bool CreateWGL();
	void ResetWGL(void (*updateFun)() = nullptr);

	void Destroy();
	bool Create(wstring name, WNDPROC wndProc, Vec2 size, Vec2 pos = { 0, 0 });
	void Render(void (*render)() = nullptr, void (*hookFun)() = nullptr);

	HWND window = nullptr;
	WNDCLASSEXW wnd = {};
	MSG msg;
	DWORD initExStyle = WS_EX_TOPMOST | WS_EX_TOOLWINDOW;
	DWORD initStyle = WS_POPUP;

	HGLRC hrc = nullptr;
	HDC glWindow = nullptr;
	PIXELFORMATDESCRIPTOR pixelFormat = {};
	Vec2 size = { 0, 0 };
	float backBufferColor[4] = { 0.f, 0.f, 0.f, 1.f };

	bool isInited = false;
	bool isRunning = true;
	bool isVisible = true;

	ImGuiIO io = {};
	ImGuiStyle style = {};
	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoResize |
								   ImGuiWindowFlags_NoMove |
								   ImGuiWindowFlags_NoScrollbar |
							       ImGuiWindowFlags_NoScrollWithMouse |
							       ImGuiWindowFlags_NoCollapse |
							       ImGuiWindowFlags_NoSavedSettings;

	string name = "";
};

inline WGL_Gui gui;