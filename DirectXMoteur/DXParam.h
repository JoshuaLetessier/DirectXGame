#pragma once

#include "pch.h"

class DXParam
{
public:

	DXParam();
	~DXParam();

	 void ParseCommandLineArguments();
	 void EnableDebugLayer();
	 void UpdateRenderTargetViews(ComPtr<ID3D12Device2> device,
		ComPtr<IDXGISwapChain4> swapChain, ComPtr<ID3D12DescriptorHeap> descriptorHeap);
	 void WaitForFenceValue(ComPtr<ID3D12Fence> fence, uint64_t fenceValue, HANDLE fenceEvent);
		//std::chrono::milliseconds duration = std::chrono::milliseconds::max());
	 void Flush(ComPtr<ID3D12CommandQueue> commandQueue, ComPtr<ID3D12Fence> fence,
		uint64_t& fenceValue, HANDLE fenceEvent);

	 bool CheckTearingSupport();

	 ComPtr<IDXGIAdapter4> GetAdapter(bool useWarp);
	 ComPtr<ID3D12Device2> CreateDevice(ComPtr<IDXGIAdapter4> adapter);
	 ComPtr<ID3D12CommandQueue> CreateCommandQueue(ComPtr<ID3D12Device2> device,
		D3D12_COMMAND_LIST_TYPE type);
	 ComPtr<IDXGISwapChain4> CreateSwapChain(HWND hWnd, ComPtr<ID3D12CommandQueue> commandQueue,
		uint32_t width, uint32_t height, uint32_t bufferCount);
	 ComPtr<ID3D12DescriptorHeap> CreateDescriptorHeap(ComPtr<ID3D12Device2> device,
		D3D12_DESCRIPTOR_HEAP_TYPE type, uint32_t numDescriptors);
	 ComPtr<ID3D12CommandAllocator> CreateCommandAllocator(ComPtr<ID3D12Device2> device,
		D3D12_COMMAND_LIST_TYPE type);   // "type"->Value typed who specify the type of allocator command create
	 ComPtr<ID3D12GraphicsCommandList> CreateCommandList(ComPtr<ID3D12Device2> device,
		ComPtr<ID3D12CommandAllocator> commandAllocator, D3D12_COMMAND_LIST_TYPE type); // "type"->Value typed who specify the type array of command create
	 ComPtr<ID3D12Fence> CreateFence(ComPtr<ID3D12Device2> device);
	 D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView() const;
	 D3D12_CPU_DESCRIPTOR_HANDLE CurrentBackBufferView() const;
	ID3D12Resource* CurrentBackBuffer() const;

	 HANDLE CreateEventHandle();

	 uint64_t Signal(ComPtr<ID3D12CommandQueue> commandQueue, ComPtr<ID3D12Fence> fence,
			uint64_t& fenceValue);


	// The number of swap chain back buffers.
	static const uint8_t g_NumFrames = 3;
	// Use WARP adapter

	 uint32_t g_ClientWidth = 1280;
	 uint32_t g_ClientHeight = 720;
	 bool g_UseWarp = false;




	// Window rectangle (used to toggle fullscreen state).
	 RECT g_WindowRect;			// g_WindowRect stores the previous dimensions of the window before transitioning to fullscreen mode, 
	// so that the window dimensions can be restored correctly when returning to windowed mode.

	// DirectX 12 Objects

	 ComPtr<ID3D12CommandQueue> g_CommandQueue;			// g_CommandQueue stores the command queue for the device.

	 ComPtr<IDXGISwapChain4> g_SwapChain;				// These resources are used for rendering images on the screen.
	// They are referenced using the DirectX 12 interface.

	ComPtr<ID3D12Resource> g_BackBuffers[g_NumFrames];	// g_BackBuffers stores pointers to the back buffer resources of the swap chain.
	
	 ComPtr<ID3D12GraphicsCommandList> g_CommandList;	// The GPU commands are first recorded into a file. Typically, only one command list is needed
	// to record GPU commands using a single thread. Since this demo uses the main thread to
	// record all GPU commands, a single command list is defined.
	// The variable is used to store the pointer to the file.

	 ComPtr<ID3D12CommandAllocator> g_CommandAllocators[g_NumFrames];	// Backup memory for recording GPU commands into a command list. 
	// Cannot be reused until all recorded commands finish executing on the GPU.
	// There must be at least one command allocator per render image.

	 ComPtr<ID3D12DescriptorHeap> g_RTVDescriptorHeap;					// Heap storing render target views for each back buffer in the swap chain.
	// Each back buffer needs a descriptor to describe it as a render target.

	 UINT g_RTVDescriptorSize;			// Size of a descriptor in a descriptor heap, which can vary depending on the hardware vendor.
												// This is needed to correctly offset into the descriptor heap.

	 UINT g_CurrentBackBufferIndex;		// Index of the current back buffer in the swap chain, which may not be sequential depending on the swap chain's flipping model.

												// Synchronization objects
	 ComPtr<ID3D12Fence> g_Fence;			// Variable used to store the fence object described in the Fence section above.

	 ComPtr<ID3D12PipelineState> g_Pso;
	
};


//C++ un membre de type ne peut pas avoir un initialiseur de classe
//C++ un membre avec un initialiseur de classe doit être const