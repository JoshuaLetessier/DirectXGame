#pragma once
#include "pch.h"


using Microsoft::WRL::ComPtr;

class MeshRenderer : public Component
{
public:
	MeshRenderer();

	void Draw();
	void BuildDescriptorHeaps();
	void BuildConstantBufferVertex();
	void BuildRootSignature();
	void BuildShader();
	void InputElement();
	void CreateCubeGeometry();
	void BuildPSO();


private:
	Cube cubeMesh;
	std::vector<std::uint16_t> m_cubeIndices = cubeIndices;
	Shape shapeMesh;
	//unsigned short* p_shapeIndices = shapeIndices;
	Pyramide pyramideMesh;
	//unsigned short* p_pyramideIndices = pyramideIndices;

	UINT elementByteSize = (sizeof(ModelViewProjectionConstantBuffer) + 255) & ~255;
	
	std::unique_ptr<MeshGeometry> mCubeGeo = nullptr;

	ComPtr<ID3DBlob> mvsByteCode = nullptr;
	ComPtr<ID3D12RootSignature> mRootSignature = nullptr;
	ComPtr<ID3D12DescriptorHeap> mCbvHeap = nullptr;


	std::unique_ptr<UploadBuffer<ModelViewProjectionConstantBuffer>> mConstantBuffer = nullptr;

	std::vector<D3D12_INPUT_ELEMENT_DESC> mInputLayout;
	bool m4xMsaaState = false;
	UINT m4xMsaaQuality = 0;
	DXGI_FORMAT mBackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
	DXGI_FORMAT mDepthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
};




