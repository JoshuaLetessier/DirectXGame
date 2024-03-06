#include "Window.h"
#include <iostream>

Window::Window()
{
}

void Window::RegisterWindowClass(HINSTANCE hInst, const wchar_t* windowClassName)
{
	LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

	// Register a window class for creating our render window with.
	WNDCLASSEXW windowClass = {};

	windowClass.cbSize = sizeof(WNDCLASSEX);                        // The size, in bytes, of this structure.
	windowClass.style = CS_HREDRAW | CS_VREDRAW;                    // Class style specifying that the entire window is redrawn if a move or size adjustment changes the width/height of the client area.
	windowClass.lpfnWndProc = &WndProc;                             // lpfnWndProc: Pointer to the Windows procedure function
	windowClass.cbClsExtra = 0;                                     // Bytes supp allocated for struct window class.
	windowClass.cbWndExtra = 0;                                     // Bytes supp allocated after instance window.
	windowClass.hInstance = hInst;                                  // hInstance: Handle to the instance that contains the window procedure for the class
	windowClass.hIcon = ::LoadIcon(hInst, NULL);                    // Icon loader Up-Left
	windowClass.hCursor = ::LoadCursor(NULL, IDC_ARROW);            // Cursor default handle
	// windowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);        // Background Color
	windowClass.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));
	windowClass.lpszMenuName = NULL;                                // Pointer to the name of the class menu resource in the resource file.
	windowClass.lpszClassName = windowClassName;                    // Pointer to a null-terminated constant string used to uniquely identify this window class.
	windowClass.hIconSm = ::LoadIcon(hInst, NULL);

	static ATOM atom = ::RegisterClassExW(&windowClass);
	assert(atom > 0);
}

void Window::Update()
{
	static uint64_t frameCounter = 0;                       // The variable stores the elapsed time in seconds since the last time the frame rate was printed.
	static double elapsedSeconds = 0.0;
	static std::chrono::high_resolution_clock clock;        // The variable is a std::chrono::time_point used to sample high-resolution time points.
	static auto t0 = clock.now();

	frameCounter++;
	auto t1 = clock.now();
	auto deltaTime = t1 - t0;
	t0 = t1;

	elapsedSeconds += deltaTime.count() * 1e-9;
	if (elapsedSeconds > 1.0)
	{
		char buffer[500];
		auto fps = frameCounter / elapsedSeconds;
		sprintf_s(buffer, 500, "FPS: %f\n", fps);

		//Converse Buffer to Char
		size_t bufferSize = strlen(buffer) + 1;
		WCHAR wBuffer[500];
		MultiByteToWideChar(CP_ACP, 0, buffer, -1, wBuffer, static_cast<int>(bufferSize));

		OutputDebugString(wBuffer);

		frameCounter = 0;
		elapsedSeconds = 0.0;
	}
}

void Window::Render()
{
	auto commandAllocator = dxParam.g_CommandAllocators[dxParam.g_CurrentBackBufferIndex];
	auto backBuffer = dxParam.g_BackBuffers[dxParam.g_CurrentBackBufferIndex];                  // Pointers to the command allocator and back buffer resource are retrieved based on the current back buffer index.                  // Pointers to the command allocator and back buffer resource are retrieved based on the current back buffer index.

	commandAllocator->Reset();                                                  // Alocator of commande and Array of commande are reset
	dxParam.g_CommandList->Reset(commandAllocator.Get(), nullptr);                      // Prepare array of commande for new register image

	// Clear the render target.
	{
		CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
			backBuffer.Get(),
			D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);      // The structure is a helper structure that facilitates easy initialization of various resource barriers.
		// In this case, the method is used to create a transition resource barrier. By default, all sub-resources will transition to the same state.
		dxParam.g_CommandList->ResourceBarrier(1, &barrier);

		FLOAT clearColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		CD3DX12_CPU_DESCRIPTOR_HANDLE rtv(dxParam.g_RTVDescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
			dxParam.g_CurrentBackBufferIndex, dxParam.g_RTVDescriptorSize);

		dxParam.g_CommandList->ClearRenderTargetView(rtv, clearColor, 0, nullptr);
	}

	// Present
	{
		CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
			backBuffer.Get(),
			D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
		dxParam.g_CommandList->ResourceBarrier(1, &barrier);

		// This method must be called in the command list before it is executed in the command queue.
		ThrowIfFailed(dxParam.g_CommandList->Close());
		ID3D12CommandList* const commandLists[] = {
			dxParam.g_CommandList.Get()
		};
		dxParam.g_CommandQueue->ExecuteCommandLists(_countof(commandLists), commandLists);              // The command list is executed on the command queue using the ExecuteCommandLists method, which takes a list of command lists to execute.

		UINT syncInterval = g_VSync ? 1 : 0;
		UINT presentFlags = g_TearingSupported && !g_VSync ? DXGI_PRESENT_ALLOW_TEARING : 0;
		ThrowIfFailed(dxParam.g_SwapChain->Present(syncInterval, presentFlags));                        // The current back buffer of the swap chain is presented to the screen using the Present method.

		g_FrameFenceValues[dxParam.g_CurrentBackBufferIndex] = dxParam.Signal(dxParam.g_CommandQueue, dxParam.g_Fence, g_FenceValue);

		dxParam.g_CurrentBackBufferIndex = dxParam.g_SwapChain->GetCurrentBackBufferIndex();                    // The method is used to get the index of the current back buffer of the swap chain.

		dxParam.WaitForFenceValue(dxParam.g_Fence, g_FrameFenceValues[dxParam.g_CurrentBackBufferIndex], g_FenceEvent); // The CPU thread is blocked until the next image overwrites
		// the content of the current back buffer, using the WaitForFenceValue function.
	}
}

void Window::Resize(uint32_t width, uint32_t height)
{
	if (g_ClientWidth != width || g_ClientHeight != height)
	{
		// Don't allow 0 size swap chain back buffers.
		g_ClientWidth = std::max(1u, width);
		g_ClientHeight = std::max(1u, height);

		// Flush the GPU queue to make sure the swap chain's back buffers
		// are not being referenced by an in-flight command list.
		dxParam.Flush(dxParam.g_CommandQueue, dxParam.g_Fence, g_FenceValue, g_FenceEvent);

		for (int i = 0; i < dxParam.g_NumFrames; ++i)
		{
			// Any references to the back buffers must be released
			// before the swap chain can be resized.
			dxParam.g_BackBuffers[i].Reset();
			g_FrameFenceValues[i] = g_FrameFenceValues[dxParam.g_CurrentBackBufferIndex];
		}

		DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
		ThrowIfFailed(dxParam.g_SwapChain->GetDesc(&swapChainDesc));
		ThrowIfFailed(dxParam.g_SwapChain->ResizeBuffers(dxParam.g_NumFrames, g_ClientWidth, g_ClientHeight,
			swapChainDesc.BufferDesc.Format, swapChainDesc.Flags));

		dxParam.g_CurrentBackBufferIndex = dxParam.g_SwapChain->GetCurrentBackBufferIndex();

		dxParam.UpdateRenderTargetViews(g_Device, dxParam.g_SwapChain, dxParam.g_RTVDescriptorHeap);
	}
}

void Window::SetFullscreen(bool fullscreen)
{
	if (g_Fullscreen != fullscreen)
	{
		g_Fullscreen = fullscreen;

		if (g_Fullscreen) // Switching to fullscreen.
		{
			// Store the current window dimensions so they can be restored
			// when switching out of fullscreen state.
			::GetWindowRect(g_hWnd, &dxParam.g_WindowRect);

			// Set the window style to a borderless window so the client area fills
			// the entire screen.
			UINT windowStyle = WS_OVERLAPPEDWINDOW & ~(WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX);
			::SetWindowLongW(g_hWnd, GWL_STYLE, windowStyle);

			// Query the name of the nearest display device for the window.
			// This is required to set the fullscreen dimensions of the window
			// when using a multi-monitor setup.
			HMONITOR hMonitor = ::MonitorFromWindow(g_hWnd, MONITOR_DEFAULTTONEAREST);
			MONITORINFOEX monitorInfo = {};
			monitorInfo.cbSize = sizeof(MONITORINFOEX);
			::GetMonitorInfo(hMonitor, &monitorInfo);

			::SetWindowPos(g_hWnd, HWND_TOP,
				monitorInfo.rcMonitor.left,
				monitorInfo.rcMonitor.top,
				monitorInfo.rcMonitor.right - monitorInfo.rcMonitor.left,
				monitorInfo.rcMonitor.bottom - monitorInfo.rcMonitor.top,
				SWP_FRAMECHANGED | SWP_NOACTIVATE);

			::ShowWindow(g_hWnd, SW_MAXIMIZE);
		}
		else
		{
			// Restore all the window decorators.
			::SetWindowLong(g_hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);

			::SetWindowPos(g_hWnd, HWND_NOTOPMOST,
				dxParam.g_WindowRect.left,
				dxParam.g_WindowRect.top,
				dxParam.g_WindowRect.right - dxParam.g_WindowRect.left,
				dxParam.g_WindowRect.bottom - dxParam.g_WindowRect.top,
				SWP_FRAMECHANGED | SWP_NOACTIVATE);

			::ShowWindow(g_hWnd, SW_NORMAL);
		}
	}
}

void Window::Init()
{
	wWinMain(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
	WndProc(hwnd, message, wParam, lParam);
	// Render();
}

LRESULT Window::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (g_IsInitialized)
	{
		switch (message)
		{
		case WM_PAINT:
			Update();
			Render();
			break;

		case WM_SYSKEYDOWN:
		case WM_KEYDOWN:
		{
			bool alt = (::GetAsyncKeyState(VK_MENU) & 0x8000) != 0;

			switch (wParam)
			{
			case 'V':
				g_VSync = !g_VSync;
				break;
			case VK_ESCAPE:
				::PostQuitMessage(0);
				break;
			case VK_RETURN:
				if (alt)
				{
			case VK_F11:
				SetFullscreen(!g_Fullscreen);
				}
				break;
			}
		}
		break;
		// The default window procedure will play a system notification sound
		// when pressing the Alt+Enter keyboard combination if this message is
		// not handled.
		case WM_SYSCHAR:
			break;

		case WM_SIZE:
		{
			RECT clientRect = {};
			::GetClientRect(g_hWnd, &clientRect);

			int width = clientRect.right - clientRect.left;
			int height = clientRect.bottom - clientRect.top;

			Resize(width, height);
		}
		break;
		case WM_DESTROY:
			::PostQuitMessage(0);
			break;
		default:
			return ::DefWindowProcW(hwnd, message, wParam, lParam);
		}
	}
	else
	{
		return ::DefWindowProcW(hwnd, message, wParam, lParam);
	}

	return 0;
}

int Window::wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR lpCmdLine, int nCmdShow)
{
	// Windows 10 Creators update adds Per Monitor V2 DPI awareness context.
	// Using this awareness context allows the client area of the window
	// to achieve 100% scaling while still allowing non-client window content to
	// be rendered in a DPI sensitive fashion.
	SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

	// Window class name. Used for registering / creating the window.
	const wchar_t* windowClassName = L"DX12WindowClass";
	dxParam.ParseCommandLineArguments();

	// Try to create hardware device.
	HRESULT hardwareResult = D3D12CreateDevice(
		nullptr,             // default adapter
		D3D_FEATURE_LEVEL_11_0,
		IID_PPV_ARGS(&meshRenderer.md3dDevice));

	//// Fallback to WARP device.
	//if (FAILED(hardwareResult))
	//{
	//    ComPtr<IDXGIAdapter> pWarpAdapter;
	//    ThrowIfFailed(mdxgiFactory->EnumWarpAdapter(IID_PPV_ARGS(&pWarpAdapter)));

	//    ThrowIfFailed(D3D12CreateDevice(
	//        pWarpAdapter.Get(),
	//        D3D_FEATURE_LEVEL_11_0,
	//        IID_PPV_ARGS(&md3dDevice)));
	//}

	dxParam.EnableDebugLayer();

	g_TearingSupported = dxParam.CheckTearingSupport();                                             // Application tearing support is queried

	RegisterWindowClass(hInstance, windowClassName);
	g_hWnd = CreateWindow(windowClassName, hInstance, L"Learning DirectX 12",
		g_ClientWidth, g_ClientHeight);

	// Initialize the global window rect variable.
	::GetWindowRect(g_hWnd, &dxParam.g_WindowRect);

	ComPtr<IDXGIAdapter4> dxgiAdapter4 = dxParam.GetAdapter(dxParam.g_UseWarp);

	g_Device = dxParam.CreateDevice(dxgiAdapter4);

	dxParam.g_CommandQueue = dxParam.CreateCommandQueue(g_Device, D3D12_COMMAND_LIST_TYPE_DIRECT);

	dxParam.g_SwapChain = dxParam.CreateSwapChain(g_hWnd, dxParam.g_CommandQueue,
		g_ClientWidth, g_ClientHeight, dxParam.g_NumFrames);

	dxParam.g_CurrentBackBufferIndex = dxParam.g_SwapChain->GetCurrentBackBufferIndex();                                    // Directly queries from the swap chain about the current back buffer index

	dxParam.g_RTVDescriptorHeap = dxParam.CreateDescriptorHeap(g_Device, D3D12_DESCRIPTOR_HEAP_TYPE_RTV, dxParam.g_NumFrames);      // Segment memory RTV create and descrive size queries
	dxParam.g_RTVDescriptorSize = g_Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	dxParam.UpdateRenderTargetViews(g_Device, dxParam.g_SwapChain, dxParam.g_RTVDescriptorHeap);

	for (int i = 0; i < dxParam.g_NumFrames; ++i)
	{
		dxParam.g_CommandAllocators[i] = dxParam.CreateCommandAllocator(g_Device, D3D12_COMMAND_LIST_TYPE_DIRECT);
	}
	dxParam.g_CommandList = dxParam.CreateCommandList(g_Device, dxParam.g_CommandAllocators[dxParam.g_CurrentBackBufferIndex], D3D12_COMMAND_LIST_TYPE_DIRECT);
	/* if (dxParam.g_CommandList == nullptr)
	 {
		 printf("dxParam.g_CommandList = nullptr %p\n", dxParam.g_CommandList);
	 }*/

	dxParam.g_Fence = dxParam.CreateFence(g_Device);                // Perform GPU synchronization
	g_FenceEvent = dxParam.CreateEventHandle();             // Handle used to block the CPU

	g_IsInitialized = true;                         // Window init

	timer.Start();

	ShowWindow(g_hWnd, SW_SHOW);

	MSG msg = {};

	bool run = true;
	while (run)
	{
		while (msg.message != WM_QUIT)
		{
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}
			else
			{
				Render();
				for (int i = 0; i < 3; i++)
				{
					Microsoft::WRL::ComPtr<ID3D12CommandAllocator> m_commandAllocators = dxParam.g_CommandAllocators[i];
					if (meshRenderer.Initialize(dxParam.g_CommandList, m_commandAllocators))
					{
						meshRenderer.Draw();
					}
					else
					{
						printf("des ours");
						return 0;
					}
				}
			}
		}

		if (timer.Update())
			SetWindowTextA(hwnd, std::to_string(timer.GetFPS()).c_str());
	}

	// Flushes the command lists and ensures all resources in transit on the GPU are processed before application termination.
	// All DirectX 12 objects held by ComPtr will be automatically cleaned up when the application closes, but this cleanup
	// should not occur until the GPU has finished using them.

	// Make sure the command queue has finished all commands before closing.
	dxParam.Flush(dxParam.g_CommandQueue, dxParam.g_Fence, g_FenceValue, g_FenceEvent);

	::CloseHandle(g_FenceEvent);

	return 0;
}

HWND Window::CreateWindow(const wchar_t* windowClassName, HINSTANCE hInst, const wchar_t* windowTitle, uint32_t width, uint32_t height)
{
	int screenWidth = ::GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = ::GetSystemMetrics(SM_CYSCREEN);

	RECT windowRect = { 0, 0, static_cast<LONG>(width), static_cast<LONG>(height) };
	::AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

	int windowWidth = windowRect.right - windowRect.left;
	int windowHeight = windowRect.bottom - windowRect.top;

	// Center the window within the screen. Clamp to 0, 0 for the top-left corner.
	int windowX = std::max<int>(0, (screenWidth - windowWidth) / 2);
	int windowY = std::max<int>(0, (screenHeight - windowHeight) / 2);
	HWND hWnd = ::CreateWindowExW(
		NULL,
		windowClassName,
		windowTitle,
		WS_OVERLAPPEDWINDOW,
		windowX,
		windowY,
		windowWidth,
		windowHeight,
		NULL,
		NULL,
		hInst,
		nullptr
	);

	assert(hWnd && "Failed to create window");

	return hWnd;
}