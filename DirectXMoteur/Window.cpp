#include "Window.h"
#include "MeshRenderer.h"
#include <iostream>
#include "UploadBuffer.h"


Window win;
MeshRenderer meshRenderer;


Window::Window()
{
}

Window::~Window()
{
    int o = 0;
}


void Window::ParseCommandLineArguments()            // Replaces some of the globally defined variables by providing command line arguments when running the application.                                          
{                                           // ParseCommandLineArguments function.
    int argc;
    wchar_t** argv = ::CommandLineToArgvW(::GetCommandLineW(), &argc);

    for (size_t i = 0; i < argc; ++i)
    {
        if (::wcscmp(argv[i], L"-w") == 0 || ::wcscmp(argv[i], L"--width") == 0) // Window Width Pixel
        {
            g_ClientWidth = ::wcstol(argv[++i], nullptr, 10);

        }
        if (::wcscmp(argv[i], L"-h") == 0 || ::wcscmp(argv[i], L"--height") == 0) // Window Height Pixel
        {
            g_ClientHeight = ::wcstol(argv[++i], nullptr, 10);
        }
        if (::wcscmp(argv[i], L"-warp") == 0 || ::wcscmp(argv[i], L"--warp") == 0) // Use palt-form Window
        {
            g_UseWarp = true;
        }
    }

    // Free memory allocated by CommandLineToArgvW
    ::LocalFree(argv);
}

void Window::EnableDebugLayer()
{
#if defined(_DEBUG)
    // Always enable the debug layer before doing anything DX12 related
    // so all possible errors generated while creating DX12 objects
    // are caught by the debug layer.
    ComPtr<ID3D12Debug> debugInterface;
    ThrowIfFailed(D3D12GetDebugInterface(IID_PPV_ARGS(&debugInterface)));
    debugInterface->EnableDebugLayer();
#endif
}

void Window::UpdateRenderTargetViews(ComPtr<ID3D12Device> device, ComPtr<IDXGISwapChain4> swapChain, ComPtr<ID3D12DescriptorHeap> descriptorHeap)
{
    auto rtvDescriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);      // This method retrieves the starting point in a descriptor heap, allowing iteration through descriptors.
    // It returns a structure that simplifies handling of this starting point.

    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(descriptorHeap->GetCPUDescriptorHandleForHeapStart());

    for (int i = 0; i < g_NumFrames; ++i)
    {
        ComPtr<ID3D12Resource> backBuffer;
        ThrowIfFailed(swapChain->GetBuffer(i, IID_PPV_ARGS(&backBuffer)));                                  // Pointer to the back buffers of the swap chain

        device->CreateRenderTargetView(backBuffer.Get(), nullptr, rtvHandle);                               // A pointer to the resource containing the render target texture and a descriptor specifying where the view is placed.

        g_BackBuffers[i] = backBuffer;                                                                      // A pointer to the buffer is also stored in the global variable.

        rtvHandle.Offset(rtvDescriptorSize);
    }
}

// Used to block the CPU thread if the fence has not yet reached a certain value. 
void Window::WaitForFenceValue(ComPtr<ID3D12Fence> fence, uint64_t fenceValue, HANDLE fenceEvent)
{
    std::chrono::milliseconds duration = std::chrono::milliseconds::max();
    if (fence->GetCompletedValue() < fenceValue)
    {
        ThrowIfFailed(fence->SetEventOnCompletion(fenceValue, fenceEvent));
        ::WaitForSingleObject(fenceEvent, static_cast<DWORD>(duration.count()));
    }
}

// The function returns the fence value to wait for. 
// It is used to wait for the fence to be signaled with a specified value.
void Window::Flush(ComPtr<ID3D12CommandQueue> commandQueue, ComPtr<ID3D12Fence> fence, uint64_t& fenceValue, HANDLE fenceEvent)
{

    // Advance the fence value to mark commands up to this fence point.
    mCurrentFence++;

    // Add an instruction to the command queue to set a new fence point.  Because we 
    // are on the GPU timeline, the new fence point won't be set until the GPU finishes
    // processing all the commands prior to this Signal().
    ThrowIfFailed(g_CommandQueue->Signal(g_Fence.Get(), mCurrentFence));

    // Wait until the GPU has completed commands up to this fence point.
    if (g_Fence->GetCompletedValue() < mCurrentFence)
    {
        LPCWSTR lpName = 0;
        HANDLE eventHandle = CreateEventExW(nullptr, lpName, false, EVENT_ALL_ACCESS);

        // Fire event when GPU hits current fence.  
        ThrowIfFailed(g_Fence->SetEventOnCompletion(mCurrentFence, eventHandle));

        // Wait until the GPU hits current fence event is fired.
        WaitForSingleObject(eventHandle, INFINITE);
        CloseHandle(eventHandle);
    }
}

bool Window::CheckTearingSupport()
{
    BOOL allowTearing = FALSE;

    // Rather than create the DXGI 1.5 factory interface directly, we create the
    // DXGI 1.4 interface and query for the 1.5 interface. This is to enable the 
    // graphics debugging tools which will not support the 1.5 factory interface 
    // until a future update.
    ComPtr<IDXGIFactory4> factory4;
    if (SUCCEEDED(CreateDXGIFactory1(IID_PPV_ARGS(&factory4))))
    {
        ComPtr<IDXGIFactory5> factory5;
        if (SUCCEEDED(factory4.As(&factory5)))
        {
            if (FAILED(factory5->CheckFeatureSupport(       // CheckFeatureSupport method of IDXGIFactory5 interface is used to query whether tearing is supported on the user's computer.
                DXGI_FEATURE_PRESENT_ALLOW_TEARING,         // The flag DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING is used to specify that tearing is allowed when presenting the swap chain with a sync interval of 0.
                &allowTearing, sizeof(allowTearing))))
            {
                allowTearing = FALSE;
            }
        }
    }

    return allowTearing == TRUE;
}

ComPtr<IDXGIAdapter4> Window::GetAdapter(bool useWarp)
{
    ComPtr<IDXGIFactory4> dxgiFactory;
    UINT createFactoryFlags = 0;
#if defined(_DEBUG)
    createFactoryFlags = DXGI_CREATE_FACTORY_DEBUG;         // Enables debug layer to detect errors during device creation and adapter querying.
#endif

    ThrowIfFailed(CreateDXGIFactory2(createFactoryFlags, IID_PPV_ARGS(&dxgiFactory)));

    ComPtr<IDXGIAdapter1> dxgiAdapter1;
    ComPtr<IDXGIAdapter4> dxgiAdapter4;

    if (useWarp)
    {
        ThrowIfFailed(dxgiFactory->EnumWarpAdapter(IID_PPV_ARGS(&dxgiAdapter1)));
        ThrowIfFailed(dxgiAdapter1.As(&dxgiAdapter4));
    }

    else
    {
        SIZE_T maxDedicatedVideoMemory = 0;
        for (UINT i = 0; dxgiFactory->EnumAdapters1(i, &dxgiAdapter1) != DXGI_ERROR_NOT_FOUND; ++i)
        {
            DXGI_ADAPTER_DESC1 dxgiAdapterDesc1;
            dxgiAdapter1->GetDesc1(&dxgiAdapterDesc1);

            // Check to see if the adapter can create a D3D12 device without actually 
            // creating it. The adapter with the largest dedicated video memory
            // is favored.
            if ((dxgiAdapterDesc1.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) == 0 &&
                SUCCEEDED(D3D12CreateDevice(dxgiAdapter1.Get(),
                    D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device), nullptr)) &&
                dxgiAdapterDesc1.DedicatedVideoMemory > maxDedicatedVideoMemory)
            {
                maxDedicatedVideoMemory = dxgiAdapterDesc1.DedicatedVideoMemory;
                ThrowIfFailed(dxgiAdapter1.As(&dxgiAdapter4));
            }
        }
    }

    return dxgiAdapter4;
}

ComPtr<ID3D12Device> Window::CreateDevice(ComPtr<IDXGIAdapter4> adapter)
{
    ComPtr<ID3D12Device> D3D12Device;
    ThrowIfFailed(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&D3D12Device)));

    HRESULT WINAPI D3D12CreateDevice(
        _In_opt_  IUnknown * pAdapter,                  //Carte video or default
        D3D_FEATURE_LEVEL MinimumFeatureLevel,
        _In_      REFIID            riid,
        _Out_opt_ void** ppDevice                       //Ptr Memory Block
    );

    // Enable debug messages in debug mode.
#if defined(_DEBUG)
    ComPtr<ID3D12InfoQueue> pInfoQueue;
    if (SUCCEEDED(D3D12Device.As(&pInfoQueue)))
    {
        pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, TRUE);        // Sets a severity level of message to break on when a message of this severity level passes through the storage filter.
        pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, TRUE);             // ERROR or WARNING messages are generated if an error or warning is generated by the debug layer.
        pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, TRUE);           // The CORRUPTION message is generated if memory corruption occurs.

        // Suppress whole categories of messages
//D3D12_MESSAGE_CATEGORY Categories[] = {};

// Suppress messages based on their severity level
        D3D12_MESSAGE_SEVERITY Severities[] =
        {
            D3D12_MESSAGE_SEVERITY_INFO
        };

        // Suppress individual messages by their ID
        D3D12_MESSAGE_ID DenyIds[] = {
            D3D12_MESSAGE_ID_CLEARRENDERTARGETVIEW_MISMATCHINGCLEARVALUE,   // I'm really not sure how to avoid this message.
            D3D12_MESSAGE_ID_MAP_INVALID_NULLRANGE,                         // This warning occurs when using capture frame while graphics debugging.
            D3D12_MESSAGE_ID_UNMAP_INVALID_NULLRANGE,                       // This warning occurs when using capture frame while graphics debugging.
        };

        D3D12_INFO_QUEUE_FILTER NewFilter = {};
        //NewFilter.DenyList.NumCategories = _countof(Categories);
        //NewFilter.DenyList.pCategoryList = Categories;
        NewFilter.DenyList.NumSeverities = _countof(Severities);
        NewFilter.DenyList.pSeverityList = Severities;
        NewFilter.DenyList.NumIDs = _countof(DenyIds);
        NewFilter.DenyList.pIDList = DenyIds;

        ThrowIfFailed(pInfoQueue->PushStorageFilter(&NewFilter));
    }
#endif

    return D3D12Device;
}

ComPtr<ID3D12CommandQueue> Window::CreateCommandQueue(ComPtr<ID3D12Device> device, D3D12_COMMAND_LIST_TYPE type)   // The command queue is created using the CreateCommandQueue method of the ID3D12Device interface,
{                                                                                                           // which takes a D3D12_COMMAND_QUEUE_DESC structure as its first argument.
    ComPtr<ID3D12CommandQueue> d3d12CommandQueue;

    D3D12_COMMAND_QUEUE_DESC desc = {};
    desc.Type = type;
    desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
    desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    desc.NodeMask = 0;

    ThrowIfFailed(device->CreateCommandQueue(&desc, IID_PPV_ARGS(&d3d12CommandQueue)));

    return d3d12CommandQueue;
}

ComPtr<IDXGISwapChain4> Window::CreateSwapChain(HWND hWnd, ComPtr<ID3D12CommandQueue> commandQueue, uint32_t width, uint32_t height, uint32_t bufferCount)
{
    ComPtr<IDXGISwapChain4> dxgiSwapChain4;
    ComPtr<IDXGIFactory4> dxgiFactory4;
    UINT createFactoryFlags = 0;
#if defined(_DEBUG)
    createFactoryFlags = DXGI_CREATE_FACTORY_DEBUG;     // Create DXGI
#endif

    ThrowIfFailed(CreateDXGIFactory2(createFactoryFlags, IID_PPV_ARGS(&dxgiFactory4)));

    DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
    swapChainDesc.Width = width;                                                                // Window Width Rpixel
    swapChainDesc.Height = height;                                                              // Window Height Rpixel
    swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;                                          // Format of render
    swapChainDesc.Stereo = FALSE;                                                               // Specifies stereo rendering for the full-screen display mode or the swap chain's back buffer.
    swapChainDesc.SampleDesc = { 1, 0 };                                                        // Structure describing multiple sampling parameters.
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;                                // Describes usage and processor access options for the back buffer.
    swapChainDesc.BufferCount = bufferCount;                                                    // Value describing the number of buffers in the swap chain.
    swapChainDesc.Scaling = DXGI_SCALING_STRETCH;                                               // Typed value identifying the resizing behavior if the back buffer size does not match the target output.
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;                                   // Presentation model and buffer content management options.
    swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;                                      // Back buffer transparency behavior.
    // It is recommended to always allow tearing if tearing support is available.
    swapChainDesc.Flags = CheckTearingSupport() ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0;       // Combination of typed values combined using bitwise OR operation.
    // The flag should always be specified if tearing support is available.
    ComPtr<IDXGISwapChain1> swapChain1;
    ThrowIfFailed(dxgiFactory4->CreateSwapChainForHwnd(
        commandQueue.Get(),
        hWnd,
        &swapChainDesc,
        nullptr,
        nullptr,
        &swapChain1));

    // Disable the Alt+Enter fullscreen toggle feature. Switching to fullscreen
    // will be handled manually.
    ThrowIfFailed(dxgiFactory4->MakeWindowAssociation(hWnd, DXGI_MWA_NO_ALT_ENTER));

    ThrowIfFailed(swapChain1.As(&dxgiSwapChain4));

    return dxgiSwapChain4;
}

// The function described above creates a descriptor heap of a specific type.
ComPtr<ID3D12DescriptorHeap>Window::CreateDescriptorHeap(ComPtr<ID3D12Device> device, D3D12_DESCRIPTOR_HEAP_TYPE type, uint32_t numDescriptors)    // "type" Value typed specify type of describes heap ; "numDescriptors"  Number of describes of heap
{
    ComPtr<ID3D12DescriptorHeap> descriptorHeap;

    D3D12_DESCRIPTOR_HEAP_DESC desc = {};
    desc.NumDescriptors = numDescriptors;
    desc.Type = type;

    ThrowIfFailed(device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&descriptorHeap)));

    return descriptorHeap;
}

ComPtr<ID3D12CommandAllocator> Window::CreateCommandAllocator(ComPtr<ID3D12Device> device,
    D3D12_COMMAND_LIST_TYPE type)   // "type"->Value typed who specify the type of allocator command create
{
    ComPtr<ID3D12CommandAllocator> commandAllocator; // Pointer to the block memory who recive a pointer to the allocator command interface
    ThrowIfFailed(device->CreateCommandAllocator(type, IID_PPV_ARGS(&commandAllocator)));

    return commandAllocator;
}

ComPtr<ID3D12GraphicsCommandList> Window::CreateCommandList(ComPtr<ID3D12Device> device,
    ComPtr<ID3D12CommandAllocator> commandAllocator, D3D12_COMMAND_LIST_TYPE type)      // "type"->Value typed who specify the type array of command create
{
    ComPtr<ID3D12GraphicsCommandList> commandList;
    ThrowIfFailed(device->CreateCommandList(0, type, commandAllocator.Get(), nullptr, IID_PPV_ARGS(&commandList)));

    ThrowIfFailed(commandList->Close());

    return commandList;
}

ComPtr<ID3D12Fence> Window::CreateFence(ComPtr<ID3D12Device> device)
{
    ComPtr<ID3D12Fence> fence;
    ThrowIfFailed(device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence)));

    return fence;
}

D3D12_CPU_DESCRIPTOR_HANDLE Window::DepthStencilView() const
{
    return g_RTVDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
}

D3D12_CPU_DESCRIPTOR_HANDLE Window::CurrentBackBufferView() const
{
    return CD3DX12_CPU_DESCRIPTOR_HANDLE(g_RTVDescriptorHeap->GetCPUDescriptorHandleForHeapStart(), g_CurrentBackBufferIndex, g_RTVDescriptorSize);
}

ID3D12Resource* Window::CurrentBackBuffer() const
{
    return g_BackBuffers[g_NumFrames].Get();
}

HANDLE Window::CreateEventHandle()
{
    HANDLE fenceEvent;

    fenceEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
    assert(fenceEvent && "Failed to create fence event.");

    return fenceEvent;
}

uint64_t Window::Signal(ComPtr<ID3D12CommandQueue> commandQueue, ComPtr<ID3D12Fence> fence, uint64_t& fenceValue)
{
    uint64_t fenceValueForSignal = ++fenceValue;
    ThrowIfFailed(g_CommandQueue->Signal(fence.Get(), fenceValueForSignal));

    return fenceValueForSignal;
}

void Window::RegisterWindowClass(HINSTANCE hInst, const wchar_t* windowClassName)
{

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
    //windowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);         // Background Color
    windowClass.hbrBackground = CreateSolidBrush(RGB(60, 5, 0));
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

    auto& commandAllocator = g_CommandAllocators;
    auto& backBuffer = g_BackBuffers[g_CurrentBackBufferIndex];                  // Pointers to the command allocator and back buffer resource are retrieved based on the current back buffer index.                  // Pointers to the command allocator and back buffer resource are retrieved based on the current back buffer index.

    commandAllocator->Reset();                                                  // Alocator of commande and Array of commande are reset
    g_CommandList->Reset(commandAllocator.Get(), nullptr);                      // Prepare array of commande for new register image

    // Clear the render target.
    {
        CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
            backBuffer.Get(),
            D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);      // The structure is a helper structure that facilitates easy initialization of various resource barriers.
        // In this case, the method is used to create a transition resource barrier. By default, all sub-resources will transition to the same state.
        g_CommandList->ResourceBarrier(1, &barrier);

        FLOAT clearColor[] = { 0.9f, 0.9f, 0.9f, 1.0f };
        CD3DX12_CPU_DESCRIPTOR_HANDLE rtv(g_RTVDescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
            g_CurrentBackBufferIndex, g_RTVDescriptorSize);

        g_CommandList->ClearRenderTargetView(rtv, clearColor, 0, nullptr);
    }

    // Present
    {
        CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
            backBuffer.Get(),
            D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
        g_CommandList->ResourceBarrier(1, &barrier);

        // This method must be called in the command list before it is executed in the command queue.
        ThrowIfFailed(g_CommandList->Close());
        ID3D12CommandList* const commandLists[] = {
            g_CommandList.Get()
        };
        g_CommandQueue->ExecuteCommandLists(_countof(commandLists), commandLists);              // The command list is executed on the command queue using the ExecuteCommandLists method, which takes a list of command lists to execute.

        UINT syncInterval = g_VSync ? 1 : 0;
        UINT presentFlags = g_TearingSupported && !g_VSync ? DXGI_PRESENT_ALLOW_TEARING : 0;
        ThrowIfFailed(g_SwapChain->Present(syncInterval, presentFlags));                        // The current back buffer of the swap chain is presented to the screen using the Present method.

        g_FrameFenceValues[g_CurrentBackBufferIndex] = Signal(g_CommandQueue, g_Fence, g_FenceValue);

        g_CurrentBackBufferIndex = g_SwapChain->GetCurrentBackBufferIndex();                    // The method is used to get the index of the current back buffer of the swap chain.

        WaitForFenceValue(g_Fence, g_FrameFenceValues[g_CurrentBackBufferIndex], g_FenceEvent); // The CPU thread is blocked until the next image overwrites 
        // the content of the current back buffer, using the WaitForFenceValue function.
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
            ::GetWindowRect(g_hWnd, &g_WindowRect);

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
                g_WindowRect.left,
                g_WindowRect.top,
                g_WindowRect.right - g_WindowRect.left,
                g_WindowRect.bottom - g_WindowRect.top,
                SWP_FRAMECHANGED | SWP_NOACTIVATE);

            ::ShowWindow(g_hWnd, SW_NORMAL);
        }
    }
}

void Window::Init()
{
    wWinMain(hInstance,hPrevInstance, lpCmdLine, nCmdShow);
    
}

LRESULT Window::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (!win.g_IsInitialized)
    {
        switch (message)
        {
       /* case WM_PAINT:
            win.Update();
            win.Render();
            break;*/

        case WM_SYSKEYDOWN:
        case WM_KEYDOWN:
        {
            bool alt = (::GetAsyncKeyState(VK_MENU) & 0x8000) != 0;

            switch (wParam)
            {
            case 'V':
                win.g_VSync = !win.g_VSync;
                break;
            case VK_ESCAPE:
                ::PostQuitMessage(0);
                break;
            case VK_RETURN:
                if (alt)
                {
            case VK_F11:
                win.SetFullscreen(!win.g_Fullscreen);
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
            ::GetClientRect(win.g_hWnd, &clientRect);

            int width = clientRect.right - clientRect.left;
            int height = clientRect.bottom - clientRect.top;

            win.Resize(width, height);
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
    ParseCommandLineArguments();

    EnableDebugLayer();

    g_TearingSupported = CheckTearingSupport();                                             // Application tearing support is queried

    RegisterWindowClass(hInstance, windowClassName);
    g_hWnd = CreateWindowE(windowClassName, hInstance, L"DirectX 12 : ",
        g_ClientWidth, g_ClientHeight);

    // Initialize the global window rect variable.
    ::GetWindowRect(g_hWnd, &g_WindowRect);

    ComPtr<IDXGIAdapter4> dxgiAdapter4 = GetAdapter(g_UseWarp);

    g_Device = CreateDevice(dxgiAdapter4);

    g_CommandQueue = CreateCommandQueue(g_Device, D3D12_COMMAND_LIST_TYPE_DIRECT);

    g_SwapChain = CreateSwapChain(g_hWnd, g_CommandQueue,
        g_ClientWidth, g_ClientHeight, g_NumFrames);

    g_CurrentBackBufferIndex = g_SwapChain->GetCurrentBackBufferIndex();                                    // Directly queries from the swap chain about the current back buffer index

    g_RTVDescriptorHeap = CreateDescriptorHeap(g_Device, D3D12_DESCRIPTOR_HEAP_TYPE_RTV, g_NumFrames);      // Segment memory RTV create and descrive size queries 
    g_RTVDescriptorSize = g_Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

    UpdateRenderTargetViews(g_Device, g_SwapChain, g_RTVDescriptorHeap);

    for (int i = 0; i < g_NumFrames; ++i)
    {
        g_CommandAllocators = CreateCommandAllocator(g_Device, D3D12_COMMAND_LIST_TYPE_DIRECT);
    }
    g_CommandList = CreateCommandList(g_Device,
        g_CommandAllocators, D3D12_COMMAND_LIST_TYPE_DIRECT);

    g_Fence = CreateFence(g_Device);                // Perform GPU synchronization
    g_FenceEvent = CreateEventHandle();             // Handle used to block the CPU

                           // Window init

    ::ShowWindow(g_hWnd, SW_SHOW);

    MSG msg = {};
    while (msg.message != WM_QUIT)
    {
        g_IsInitialized = true;
        if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
        }
        else
        {
            //Render();
           
            WndProc(hwnd, message, wParam, lParam);
            for (int i = 0; i < g_NumFrames; i++)
            {
                Microsoft::WRL::ComPtr<ID3D12CommandAllocator> m_commandAllocators = g_CommandAllocators;
                
                ThrowIfFailed(g_CommandList->Reset(m_commandAllocators.Get(), nullptr));
                BuildDescriptorHeaps();
                BuildConstantBufferVertex();
                BuildRootSignature();
                meshRenderer.BuildShader();
                InputElement();
                BuildPSO();
                MeshUpdate();
                Draw();

                ThrowIfFailed(g_CommandList->Close());
                ID3D12CommandList* cmdsLists[] = {g_CommandList.Get() };
                g_CommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

                // Wait until initialization is complete.
                Flush(g_CommandQueue,g_Fence, g_FenceValue, g_FenceEvent);

            }
        }
    }

    // Flushes the command lists and ensures all resources in transit on the GPU are processed before application termination.
    // All DirectX 12 objects held by ComPtr will be automatically cleaned up when the application closes, but this cleanup
    // should not occur until the GPU has finished using them.

    // Make sure the command queue has finished all commands before closing.
    

    ::CloseHandle(g_FenceEvent);

    return 0;
}

HWND Window::CreateWindowE(const wchar_t* windowClassName, HINSTANCE hInst, const wchar_t* windowTitle, uint32_t width, uint32_t height)
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

void Window::Resize(uint32_t width, uint32_t height)
{
    if (g_ClientWidth != width || g_ClientHeight != height)
    {
        // Don't allow 0 size swap chain back buffers.
        g_ClientWidth = std::max(1u, width);
        g_ClientHeight = std::max(1u, height);

        // Flush the GPU queue to make sure the swap chain's back buffers
        // are not being referenced by an in-flight command list.
        printf("test %p\n", g_CommandQueue);
        Flush(g_CommandQueue, g_Fence, g_FenceValue, g_FenceEvent);

        ThrowIfFailed(g_CommandList->Reset(g_CommandAllocators.Get(), nullptr));

        for (int i = 0; i < g_NumFrames; ++i)
        {
            // Any references to the back buffers must be released
            // before the swap chain can be resized.
            g_BackBuffers[i].Reset();
            g_FrameFenceValues[i] = g_FrameFenceValues[g_CurrentBackBufferIndex];
        }

        DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
        ThrowIfFailed(g_SwapChain->GetDesc(&swapChainDesc));
        {
            HRESULT hr__ = (g_SwapChain->ResizeBuffers(g_NumFrames, g_ClientWidth, g_ClientHeight, swapChainDesc.BufferDesc.Format, swapChainDesc.Flags)); std::wstring wfn = AnsiToWString("C:\\Users\\Askeladd\\Desktop\\CODE\\DirectXGame\\DirectXMoteur\\Window.cpp"); if ((((HRESULT)(hr__)) < 0)) {
                throw DxException(hr__, L"g_SwapChain->ResizeBuffers(g_NumFrames, g_ClientWidth, g_ClientHeight, swapChainDesc.BufferDesc.Format, swapChainDesc.Flags)", wfn, 137);
            }
        };

        g_CurrentBackBufferIndex = g_SwapChain->GetCurrentBackBufferIndex();

        UpdateRenderTargetViews(g_Device, g_SwapChain, g_RTVDescriptorHeap);
    }
}



void Window::BuildDescriptorHeaps()
{
    D3D12_DESCRIPTOR_HEAP_DESC cbvheapDesc;
    cbvheapDesc.NumDescriptors = g_NumFrames;
    cbvheapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    cbvheapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    cbvheapDesc.NodeMask = 0;

    CreateDescriptorHeap(g_Device, cbvheapDesc.Type, cbvheapDesc.NumDescriptors);

}

void Window::BuildConstantBufferVertex()
{
    if (!g_RTVDescriptorHeap)
    {
        D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
        rtvHeapDesc.NumDescriptors = 3; // tailleDuTampon
        rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
        rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

        HRESULT hr = g_Device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&g_RTVDescriptorHeap));
        if (FAILED(hr))
        {
            OutputDebugString(L"Erreur lors de la création du tampon de descripteur.\n");
            std::cerr << "Erreur lors de la création du tampon de descripteur : " << hr << std::endl;
            return;
        }
    }

    mConstantBuffer = std::make_unique<UploadBuffer<ModelViewProjectionConstantBuffer>>(g_Device.Get(), 1, true);

    //UINT mCBByteSize = d3dUtil::CalcConstantBufferByteSize(sizeof(ModelViewProjectionConstantBuffer));

    //D3D12_GPU_VIRTUAL_ADDRESS cbAddress = mConstantBuffer->Resource()->GetGPUVirtualAddress();

    //int boxCBufIndex = 0;
    //cbAddress += boxCBufIndex * mCBByteSize;

    //D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc;
    //cbvDesc.BufferLocation = cbAddress;
    //cbvDesc.SizeInBytes = sizeof(ModelViewProjectionConstantBuffer);

    //UINT descriptorSize = g_Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    //D3D12_CPU_DESCRIPTOR_HANDLE value = g_RTVDescriptorHeap->GetCPUDescriptorHandleForHeapStart();

    //// Vérifie si la handle de descripteur est dans la plage valide du tampon
    //if (value.ptr < g_RTVDescriptorHeap->GetCPUDescriptorHandleForHeapStart().ptr ||
    //    value.ptr >= g_RTVDescriptorHeap->GetCPUDescriptorHandleForHeapStart().ptr + descriptorSize * 3)
    //{
    //    OutputDebugString(L"Erreur : La handle de descripteur ne pointe pas vers un emplacement valide dans le tampon de descripteur.\n");
    //    return;
    //}

    //// Crée la vue de tampon constant
    //g_Device->CreateConstantBufferView(&cbvDesc, value);
}

void Window::BuildRootSignature()
{
    CD3DX12_ROOT_PARAMETER slotRootParem[1];

    slotRootParem[0].InitAsConstantBufferView(0);

    CD3DX12_ROOT_SIGNATURE_DESC rootSign(1, slotRootParem, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

    ComPtr<ID3D10Blob> serializedRootSig = nullptr;
    ComPtr<ID3D10Blob> errorBlob = nullptr;

    HRESULT hr = D3D12SerializeRootSignature(&rootSign, D3D_ROOT_SIGNATURE_VERSION_1_0, serializedRootSig.GetAddressOf(), errorBlob.GetAddressOf());
    if (errorBlob != nullptr)
        ::OutputDebugStringA((char*)errorBlob->GetBufferPointer());//les deux point pour dire que c'est une fonction globale
    ThrowIfFailed(hr);

    ThrowIfFailed(g_Device->CreateRootSignature(0, serializedRootSig->GetBufferPointer(), serializedRootSig->GetBufferSize(), IID_PPV_ARGS(&mRootSignature))); /*erreur à traiter*/
}

void Window::CreateCubeGeometry()
{
    const UINT  sizeCubeMesh = sizeof(cubeMesh);
    const UINT  sizeIndicesCubeMesh = sizeof(m_cubeIndices);
    const UINT c_vertexBufferSize = sizeCubeMesh * sizeof(VertexPositionColor);
    const UINT c_indicesBufferSize = sizeIndicesCubeMesh * sizeof(std::uint16_t);

    mCubeGeo = std::make_unique<MeshGeometry>();
    mCubeGeo->Name = "cubeGeo";

    ThrowIfFailed(D3DCreateBlob(c_vertexBufferSize, &mCubeGeo->VertexBufferCPU));
    CopyMemory(mCubeGeo->VertexBufferCPU->GetBufferPointer(), cubeMesh.cubeVertices.data(), c_vertexBufferSize);

    ThrowIfFailed(D3DCreateBlob(c_indicesBufferSize, &mCubeGeo->IndexBufferCPU));
    CopyMemory(mCubeGeo->IndexBufferCPU->GetBufferPointer(), m_cubeIndices.data(), c_indicesBufferSize);

    mCubeGeo->VertexBufferGPU = d3dUtil::CreateDefaultBuffer(g_Device.Get(), g_CommandList.Get(), cubeMesh.cubeVertices.data(), c_vertexBufferSize, mCubeGeo->VertexBufferUploader);//vertex
    mCubeGeo->IndexBufferGPU = d3dUtil::CreateDefaultBuffer(g_Device.Get(), g_CommandList.Get(), m_cubeIndices.data(), c_indicesBufferSize, mCubeGeo->IndexBufferUploader);//index

    mCubeGeo->VertexByteStride = sizeof(VertexPositionColor);
    mCubeGeo->VertexBufferByteSize = c_vertexBufferSize;
    mCubeGeo->IndexFormat = DXGI_FORMAT_R16_UINT;
    mCubeGeo->IndexBufferByteSize = c_indicesBufferSize;

    SubmeshGeometry submesh;
    submesh.IndexCount = (UINT)sizeof(m_cubeIndices);
    submesh.StartIndexLocation = 0;
    submesh.BaseVertexLocation = 0;

    mCubeGeo->DrawArgs["box"] = submesh;

}

void Window::InputElement()
{
    mInputLayout =
    {
        {"POSITION",0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
        {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
    };
}

void Window::BuildPSO()
{
    D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc;
    ZeroMemory(&psoDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
    psoDesc.InputLayout = { mInputLayout.data(), (UINT)mInputLayout.size() };

    psoDesc.pRootSignature = mRootSignature.Get();
    psoDesc.VS =
    {
     reinterpret_cast<BYTE*>(meshRenderer.mvsByteCode->GetBufferPointer()),
     meshRenderer.mvsByteCode->GetBufferSize()
    };
    psoDesc.PS =
    {
     reinterpret_cast<BYTE*>(meshRenderer.mpsByteCode->GetBufferPointer()),
     meshRenderer.mpsByteCode->GetBufferSize()
    };
    psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
    psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
    psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
    psoDesc.SampleMask = UINT_MAX;
    psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    psoDesc.NumRenderTargets = 3;
    psoDesc.RTVFormats[0] = mBackBufferFormat;
    psoDesc.SampleDesc.Count = m4xMsaaState ? 4 : 1;
    psoDesc.SampleDesc.Quality = m4xMsaaState ? (m4xMsaaQuality - 1) : 0;
    psoDesc.DSVFormat = mDepthStencilFormat;

    ThrowIfFailed(g_Device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&g_Pso)));
}

void Window::MeshUpdate()
{
    // Convert Spherical to Cartesian coordinates.
    float x = mRadius * sinf(mPhi) * cosf(mTheta);
    float z = mRadius * sinf(mPhi) * sinf(mTheta);
    float y = mRadius * cosf(mPhi);
    // Build the view matrix.
    XMVECTOR pos = XMVectorSet(x, y, z, 1.0f);
    XMVECTOR target = XMVectorZero();
    XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    XMMATRIX view = XMMatrixLookAtLH(pos, target, up);
    XMStoreFloat4x4(&mView, view);
    XMMATRIX world = XMLoadFloat4x4(&mWorld);
    XMMATRIX proj = XMLoadFloat4x4(&mProj);
    XMMATRIX worldViewProj = world * view * proj;
    // Update the constant buffer with the latest worldViewProj matrix.
    ModelViewProjectionConstantBuffer objConstants;
    XMStoreFloat4x4(&objConstants.WorldViewProj, XMMatrixTranspose(worldViewProj));
    mConstantBuffer->CopyData(0, objConstants);
}

void Window::Draw()
{
    CD3DX12_RESOURCE_BARRIER transition = CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
    printf("test %p\n", g_CommandAllocators);

    ThrowIfFailed(g_CommandAllocators->Reset());

    ThrowIfFailed(g_CommandList->Reset(g_CommandAllocators.Get(), g_Pso.Get()));

    g_CommandList->RSSetViewports(1, &mScreenViewport);
    g_CommandList->RSSetScissorRects(1, &mScissorRect);

    g_CommandList->ResourceBarrier(1, &transition);

    g_CommandList->ClearRenderTargetView(CurrentBackBufferView(), Colors::LightSteelBlue, 0, nullptr);
    g_CommandList->ClearDepthStencilView(DepthStencilView(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);

    D3D12_CPU_DESCRIPTOR_HANDLE currentBackBufferView = CurrentBackBufferView();
    D3D12_CPU_DESCRIPTOR_HANDLE depthStencilView = DepthStencilView();
    g_CommandList->OMSetRenderTargets(1, &currentBackBufferView, true, &depthStencilView);


    ID3D12DescriptorHeap* descriptorHeaps[] = { g_RTVDescriptorHeap.Get() };
    g_CommandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);
    g_CommandList->SetGraphicsRootSignature(mRootSignature.Get());

    D3D12_VERTEX_BUFFER_VIEW vertexBufferView = mCubeGeo->VertexBufferView();
    g_CommandList->IASetVertexBuffers(0, 1, &vertexBufferView);

    D3D12_INDEX_BUFFER_VIEW indexBufferView = mCubeGeo->IndexBufferView();
    g_CommandList->IASetIndexBuffer(&indexBufferView);
    g_CommandList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    g_CommandList->SetGraphicsRootConstantBufferView(0, mConstantBuffer->Resource()->GetGPUVirtualAddress());
    g_CommandList->SetPipelineState(g_Pso.Get());
    g_CommandList->DrawIndexedInstanced(mCubeGeo->DrawArgs["box"].IndexCount, 1, 0, 0, 0);


    g_CommandList->ResourceBarrier(1, &transition);

    ThrowIfFailed(g_CommandList->Close());

    ID3D12CommandList* cmdsLists[] = { g_CommandList.Get() };
    g_CommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);


    ThrowIfFailed(g_SwapChain->Present(0, 0));
    mCurrBackBuffer = (mCurrBackBuffer + 1) % SwapChainBufferCount;


    Flush(g_CommandQueue, g_Fence, g_FenceValue, g_FenceEvent);
}