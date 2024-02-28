#pragma once
#include "pch.h"
#include "DXParam.h"

class Window
{
private:

	// Window callback function.
	LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

public:
	void RegisterWindowClass(HINSTANCE hInst, const wchar_t* windowClassName);
	void Update();
	void Render();
	void Resize(uint32_t width, uint32_t height);
	void SetFullscreen(bool fullscreen);
	LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	int CALLBACK wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR lpCmdLine, int nCmdShow);


	HWND CreateWindow(const wchar_t* windowClassName, HINSTANCE hInst,
		const wchar_t* windowTitle, uint32_t width, uint32_t height);
};

