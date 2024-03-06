#pragma once
#include "pch.h"

using Microsoft::WRL::ComPtr;
using namespace renderObject;

class MeshRenderer : public Component
{
public:
	MeshRenderer();

	bool Initialize(ComPtr<ID3D12GraphicsCommandList> m_CommandList, Microsoft::WRL::ComPtr<ID3D12CommandAllocator> m_CommandAllocators);

	//void OnResize();
	//void Update();
	void Draw();

	void Update();
	void BuildDescriptorHeaps();
	void BuildConstantBufferVertex();
	void BuildRootSignature();
	void BuildShader();
	void InputElement();
	void CreateCubeGeometry();
	void BuildPSO();

	Microsoft::WRL::ComPtr<ID3D12Device> md3dDevice;
	ComPtr<ID3DBlob> mvsByteCode = nullptr;
	ComPtr < ID3DBlob> mpsByteCode = nullptr;
private:
	Cube cubeMesh;
	std::vector<std::uint16_t> m_cubeIndices = cubeIndices;
	//Shape shapeMesh;
	//unsigned short* p_shapeIndices = shapeIndices;
	//Pyramide pyramideMesh;
	//unsigned short* p_pyramideIndices = pyramideIndices;



	UINT elementByteSize = (sizeof(ModelViewProjectionConstantBuffer) + 255) & ~255;

	std::unique_ptr<MeshGeometry> mCubeGeo = nullptr;


	ComPtr<ID3D12RootSignature> mRootSignature = nullptr;
	ComPtr<ID3D12DescriptorHeap> mCbvHeap = nullptr;
	ComPtr<ID3D12CommandQueue> mCommandQueue;

	std::unique_ptr<UploadBuffer<ModelViewProjectionConstantBuffer>> mConstantBuffer = nullptr;

	std::vector<D3D12_INPUT_ELEMENT_DESC> mInputLayout;
	bool m4xMsaaState = false;
	UINT m4xMsaaQuality = 0;
	DXGI_FORMAT mBackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
	DXGI_FORMAT mDepthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	int mCurrBackBuffer = 0;
	static const int SwapChainBufferCount = 2;



	D3D12_VIEWPORT mScreenViewport;
	D3D12_RECT mScissorRect;


	XMFLOAT4X4 mWorld = MathHelper::Identity4x4();
	XMFLOAT4X4 mView = MathHelper::Identity4x4();
	XMFLOAT4X4 mProj = MathHelper::Identity4x4();

	float mTheta = 1.5f * XM_PI;
	float mPhi = XM_PIDIV4;
	float mRadius = 5.0f;
	

};
