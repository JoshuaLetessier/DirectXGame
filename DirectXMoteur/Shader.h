#pragma once
#include "pch.h"
#include <unordered_map>
#include "MathHelper.h"
#include "Component.h"

const CD3DX12_STATIC_SAMPLER_DESC pointWrap(
	0, // shaderRegister
	D3D12_FILTER_MIN_MAG_MIP_POINT, // filter
	D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressU
	D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressV
	D3D12_TEXTURE_ADDRESS_MODE_WRAP); // addressW

class Shader : public Component
{
public:
	Shader();
	virtual ~Shader();

	bool Initialize() override;
	bool Initialize(Microsoft::WRL::ComPtr<ID3D12Device> device);
	void Update();

	void BuildShadersAndInputLayout();
	void BuildPSO(Microsoft::WRL::ComPtr<ID3D12Device> device, bool m4xMsaaState, UINT m4xMsaaQuality);

	ComPtr<ID3DBlob> mvsByteCode = nullptr;
	ComPtr<ID3DBlob> mpsByteCode = nullptr;

	std::vector<D3D12_INPUT_ELEMENT_DESC> mInputLayout;
	ComPtr<ID3D12RootSignature> m_rootSignature = nullptr;
	ComPtr<ID3D12PipelineState> mPSO = nullptr;
private:
};
