#include "gui.h"

void HookFun(WGL_Gui& gui) {

	if (GetAsyncKeyState(VK_MENU) & 1) {
		gui.isVisible = !gui.isVisible;
	}
}

void Render() {

	ImGui::Button("Click me!!!");
}

int main() {

	WGL_Gui gui;

	ShowWindow(GetConsoleWindow(), SW_HIDE);

	Vec2 size = { 500, 300 };
	Vec2 matrix = { GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN) };
	Vec2 pos = { matrix.x / 2 - size.x / 2, matrix.y / 2 - size.y / 2 };

	if (!gui.Create(L"Test WGL Gui", DefaultMovingWndProc, size, pos)) {

		return -1;
	}

	gui.Render(Render, HookFun);

	gui.Destroy();

	return 0;
}