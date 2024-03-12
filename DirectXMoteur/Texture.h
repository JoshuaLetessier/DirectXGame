#pragma once
#include "pch.h"

class Texture2D
{
public:
	Texture2D();
	~Texture2D();

	void Initialize(ID3D12Device* device, ID3D12CommandQueue* commandQueue, ID3D12CommandAllocator* commandAllocator, ID3D12GraphicsCommandList* commandList, ID3D12DescriptorHeap* cbvHeap);
	void CreateTexture(const wchar_t* fileName);

			private:

	ComPtr<ID3D12RootSignature> m_pRootSignature = nullptr;

	ID3D12Device* m_pDevice = nullptr;
	ID3D12Fence* m_pFence = nullptr;

	UINT64 m_CurrentFence = 0;

	ID3D12GraphicsCommandList* m_pCommandList = nullptr;
	ID3D12CommandQueue* m_pCommandQueue = nullptr;
	ID3D12CommandAllocator* m_pDirectCmdListAlloc = nullptr;

	ID3D12DescriptorHeap* m_pCbvHeap = nullptr;

	ID3D12Resource* m_Texture = nullptr;

	UINT m_textureIndex = 0;

	ComPtr<ID3DBlob> signature;
	ComPtr<ID3DBlob> error;

	void FlushCommandQueue();
};
