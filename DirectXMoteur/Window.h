#pragma once
#include "pch.h"
//#include "DXParam.h"

class Window
{
private:
	// The number of swap chain back buffers.
	static const uint8_t g_NumFrames = 1;
	// Use WARP adapter
	bool g_UseWarp = false;

	uint32_t g_ClientWidth = 1280;
	uint32_t g_ClientHeight = 720;

	// Set to true once the DX12 objects have been initialized.
	bool g_IsInitialized = false;

	// Window handle.
	HWND g_hWnd;				// g_hWnd stores a handle to the operating system window, used for rendering.

public:
	uint32_t g_ClientWidth = 1280;
	uint32_t g_ClientHeight = 720;

private:
	// Set to true once the DX12 objects have been initialized.
	bool g_IsInitialized = false;
	// By default, enable V-Sync.
	// Can be toggled with the V key.
	bool g_VSync = true;					// Controls whether the current swap chain method waits for the next vertical refresh before presenting the rendered image.
	bool g_TearingSupported = false;		// By default, it blocks until the next vertical refresh, limiting the frame rate to the screen's refresh rate.
	// Setting it to false presents the image as quickly as possible, allowing an unlimited frame rate, but may cause screen tearing.

	HINSTANCE hInstance;
	HINSTANCE hPrevInstance;
	PWSTR lpCmdLine;
	int nCmdShow;

	HWND hwnd;
	UINT message;
	WPARAM wParam;
	LPARAM lParam;

	MeshRenderer meshRenderer;
public:
	Window();
	~Window();

	void RegisterWindowClass(HINSTANCE hInst, const wchar_t* windowClassName);
	void Update();
	void Render();
	void Resize(uint32_t width, uint32_t height);
	void SetFullscreen(bool fullscreen);
	void Init();
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	int CALLBACK wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR lpCmdLine, int nCmdShow);


	HWND CreateWindowE(const wchar_t* windowClassName, HINSTANCE hInst,
		const wchar_t* windowTitle, uint32_t width, uint32_t height);


	

	void ParseCommandLineArguments();
	void EnableDebugLayer();
	void UpdateRenderTargetViews(ComPtr<ID3D12Device> device,
		ComPtr<IDXGISwapChain4> swapChain, ComPtr<ID3D12DescriptorHeap> descriptorHeap);
	void WaitForFenceValue(ComPtr<ID3D12Fence> fence, uint64_t fenceValue, HANDLE fenceEvent);
	//std::chrono::milliseconds duration = std::chrono::milliseconds::max());
	void Flush(ComPtr<ID3D12CommandQueue> commandQueue, ComPtr<ID3D12Fence> fence,
		uint64_t& fenceValue, HANDLE fenceEvent);

	bool CheckTearingSupport();

	ComPtr<IDXGIAdapter4> GetAdapter(bool useWarp);
	ComPtr<ID3D12Device> CreateDevice(ComPtr<IDXGIAdapter4> adapter);
	void BuildDescriptorHeaps();
	void BuildConstantBufferVertex();
	void BuildRootSignature();
	void CreateCubeGeometry();
	void InputElement();
	void BuildPSO();

	void Draw();
	void MeshUpdate();

	ComPtr<ID3D12CommandQueue> CreateCommandQueue(ComPtr<ID3D12Device> device,
		D3D12_COMMAND_LIST_TYPE type);
	ComPtr<IDXGISwapChain4> CreateSwapChain(HWND hWnd, ComPtr<ID3D12CommandQueue> commandQueue,
		uint32_t width, uint32_t height, uint32_t bufferCount);
	ComPtr<ID3D12DescriptorHeap> CreateDescriptorHeap(ComPtr<ID3D12Device> device,
		D3D12_DESCRIPTOR_HEAP_TYPE type, uint32_t numDescriptors);
	ComPtr<ID3D12CommandAllocator> CreateCommandAllocator(ComPtr<ID3D12Device> device,
		D3D12_COMMAND_LIST_TYPE type);   // "type"->Value typed who specify the type of allocator command create
	ComPtr<ID3D12GraphicsCommandList> CreateCommandList(ComPtr<ID3D12Device> device,
		ComPtr<ID3D12CommandAllocator> commandAllocator, D3D12_COMMAND_LIST_TYPE type); // "type"->Value typed who specify the type array of command create
	ComPtr<ID3D12Fence> CreateFence(ComPtr<ID3D12Device> device);
	D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView() const;
	D3D12_CPU_DESCRIPTOR_HANDLE CurrentBackBufferView() const;
	ID3D12Resource* CurrentBackBuffer() const;

	HANDLE CreateEventHandle();

	uint64_t Signal(ComPtr<ID3D12CommandQueue> commandQueue, ComPtr<ID3D12Fence> fence,
		uint64_t& fenceValue);


	// DirectX 12 Objects
	ComPtr<ID3D12Device> g_Device;										// g_Device stores the DirectX 12 device object.

	ComPtr<ID3D12CommandQueue> g_CommandQueue;							// g_CommandQueue stores the command queue for the device.

	ComPtr<IDXGISwapChain4> g_SwapChain;								// These resources are used for rendering images on the screen.
	// They are referenced using the DirectX 12 interface.

	ComPtr<ID3D12Resource> g_BackBuffers[g_NumFrames];					// g_BackBuffers stores pointers to the back buffer resources of the swap chain.

	ComPtr<ID3D12GraphicsCommandList> g_CommandList;					// The GPU commands are first recorded into a file. Typically, only one command list is needed
	// to record GPU commands using a single thread. Since this demo uses the main thread to
	// record all GPU commands, a single command list is defined.
	// The variable is used to store the pointer to the file.

	ComPtr<ID3D12CommandAllocator> g_CommandAllocators;	// Backup memory for recording GPU commands into a command list. 
	// Cannot be reused until all recorded commands finish executing on the GPU.
	// There must be at least one command allocator per render image.

	ComPtr<ID3D12DescriptorHeap> g_RTVDescriptorHeap;					// Heap storing render target views for each back buffer in the swap chain.
	// Each back buffer needs a descriptor to describe it as a render target.

	UINT g_RTVDescriptorSize;											// Size of a descriptor in a descriptor heap, which can vary depending on the hardware vendor.
	// This is needed to correctly offset into the descriptor heap.

	UINT g_CurrentBackBufferIndex;										// Index of the current back buffer in the swap chain, which may not be sequential depending on the swap chain's flipping model.

	// Synchronization objects
	ComPtr<ID3D12Fence> g_Fence;										// Variable used to store the fence object described in the Fence section above.

	uint64_t g_FenceValue = 0;											// The next fence value to signal the next command queue is stored in the variable.

	uint64_t g_FrameFenceValues[g_NumFrames] = {};						// For each rendered image that could be "in-flight" in the command queue,
	// the fence value that was used to signal the command queue must be tracked
	// to ensure that all resources still referenced by the command queue are not overwritten.

	HANDLE g_FenceEvent;												// Variable used as a handle to a system event object for receiving notification
	// when a fence reaches a specific value.

	Microsoft::WRL::ComPtr<IDXGISwapChain> mSwapChain;
	Microsoft::WRL::ComPtr<IDXGIFactory4> mdxgiFactory;
	// By default, enable V-Sync.
	// Can be toggled with the V key.

	bool g_VSync = true;												// Controls whether the current swap chain method waits for the next vertical refresh before presenting the rendered image.
	bool g_TearingSupported = false;									// By default, it blocks until the next vertical refresh, limiting the frame rate to the screen's refresh rate.
	// Setting it to false presents the image as quickly as possible, allowing an unlimited frame rate, but may cause screen tearing.

	// By default, use windowed mode.
	// Can be toggled with the Alt+Enter or F11
	bool g_Fullscreen = false;

	std::unique_ptr<UploadBuffer<ModelViewProjectionConstantBuffer>> mConstantBuffer = nullptr;
	ComPtr<ID3D12RootSignature> mRootSignature = nullptr;

	ComPtr<ID3D12PipelineState> g_Pso;

	Cube cubeMesh;
	std::vector<std::uint16_t> m_cubeIndices = cubeIndices;

	std::unique_ptr<MeshGeometry> mCubeGeo = nullptr;

	std::vector<D3D12_INPUT_ELEMENT_DESC> mInputLayout;
	DXGI_FORMAT mBackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
	DXGI_FORMAT mDepthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	bool m4xMsaaState = false;
	UINT m4xMsaaQuality = 0;

	D3D12_VIEWPORT mScreenViewport;
	D3D12_RECT mScissorRect;
	int mCurrBackBuffer = 0;
	static const int SwapChainBufferCount = 2;

	XMFLOAT4X4 mWorld = MathHelper::Identity4x4();
	XMFLOAT4X4 mView = MathHelper::Identity4x4();
	XMFLOAT4X4 mProj = MathHelper::Identity4x4();

	float mTheta = 1.5f * XM_PI;
	float mPhi = XM_PIDIV4;
	float mRadius = 5.0f;

	UINT64 mCurrentFence = 0;
};
