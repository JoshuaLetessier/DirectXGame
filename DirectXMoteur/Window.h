#pragma once
#include "pch.h"
#include "DXParam.h"

class Window
{
private:

	// Window callback function.
	uint64_t g_FenceValue = 0;					// The next fence value to signal the next command queue is stored in the variable.
	uint64_t g_FrameFenceValues[DXParam::g_NumFrames] = {};				// For each rendered image that could be "in-flight" in the command queue,
																// the fence value that was used to signal the command queue must be tracked
																// to ensure that all resources still referenced by the command queue are not overwritten.

	HANDLE g_FenceEvent;		// Variable used as a handle to a system event object for receiving notification
								// when a fence reaches a specific value.

	ComPtr<ID3D12Device2> g_Device;				// g_Device stores the DirectX 12 device object.

	// Window handle.
	HWND g_hWnd;				// g_hWnd stores a handle to the operating system window, used for rendering.

	uint32_t g_ClientWidth = 1280;
	uint32_t g_ClientHeight = 720;


	// Set to true once the DX12 objects have been initialized.
	bool g_IsInitialized = false;
	// By default, enable V-Sync.
	// Can be toggled with the V key.
	bool g_VSync = true;					// Controls whether the current swap chain method waits for the next vertical refresh before presenting the rendered image.
	bool g_TearingSupported = false;		// By default, it blocks until the next vertical refresh, limiting the frame rate to the screen's refresh rate.
	// Setting it to false presents the image as quickly as possible, allowing an unlimited frame rate, but may cause screen tearing.

	// By default, use windowed mode.
	// Can be toggled with the Alt+Enter or F11
	bool g_Fullscreen = false;

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

