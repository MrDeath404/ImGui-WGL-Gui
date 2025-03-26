#include "gui.h"
#include <imgui_impl_opengl3.h>
#include <imgui_impl_win32.h>

void WGL_Gui::Destroy() {

	if (isInited) {
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
		isInited = false;
	}

	DestroyWGL();
	wglDeleteContext(hrc);
	DestroyExWindow();
}

bool WGL_Gui::Create(wstring name, WNDPROC wndProc, Vec2 size, Vec2 pos) {

	if (!CreateExWindow(name, wndProc, size, pos)) {
		return false;
	}

	if (window == nullptr) { return false; }

	if (!CreateWGL()) {
		return false;
	}

	ShowWindow(window, SW_SHOWNORMAL);
	UpdateWindow(window);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ZeroMemory(&io, sizeof(io));
	ZeroMemory(&style, sizeof(style));

	io = ImGui::GetIO();
	style = ImGui::GetStyle();

	ImGui::StyleColorsClassic();

	ImGui_ImplWin32_InitForOpenGL(window);
	ImGui_ImplOpenGL3_Init();
	
	ImFont* font = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Arial.ttf", 16.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
	IM_ASSERT(font != nullptr);

	isInited = true;

	return true;
}

void WGL_Gui::Render(void (*render)(), void (*hookFun)(WGL_Gui& gui)) {

	bool visible = isVisible;

	while (true) {

		while (PeekMessageW(&msg, nullptr, 0U, 0U, PM_REMOVE)) {

			TranslateMessage(&msg);
			DispatchMessageW(&msg);
			if (msg.message == WM_QUIT) { isRunning = false; }
		}

		if (!isRunning) { break; }

		if (IsIconic(window)) {

			this_thread::sleep_for(chrono::seconds(10));
			continue;
		}

		if (visible != isVisible) {

			ShowWindow(window, isVisible ? SW_SHOW : SW_HIDE);
			visible = isVisible;
		}

		if (hookFun != nullptr) { hookFun(*this); };

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		if (isVisible) {

			ImGui::SetNextWindowPos({ 0, 0 });
			ImGui::SetNextWindowSize(size.ToImVec2());

			ImGui::Begin(name.c_str(), &isRunning, windowFlags);
			if (render != nullptr) { render(); }
			ImGui::End();
		}

		ImGui::Render();
		glViewport(0, 0, size.x, size.y);
		glClearColor(backBufferColor[0], backBufferColor[1], backBufferColor[2], backBufferColor[3]);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		SwapBuffers(glWindow);
	}
}