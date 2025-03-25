#include "gui.h"

int main() {

	ShowWindow(GetConsoleWindow(), SW_HIDE);

	if (!gui.Create(L"Test WGL Gui", DefaultMovingWndProc, { 500, 400 })) {

		return -1;
	}

	gui.Render();

	gui.Destroy();

	return 0;
}