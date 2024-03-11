#pragma once
#include "pch.h"
#include <unordered_map>
#include "MathHelper.h"
#include "Component.h"

class Shader: public Component
{

public:
	Shader();
	virtual ~Shader();

	bool Initialize() override;
	bool Initialize(Microsoft::WRL::ComPtr<ID3D12Device> device);
	void Update();

	void BuildShadersAndInputLayout();


	ComPtr<ID3DBlob> mvsByteCode = nullptr;
	ComPtr<ID3DBlob> mpsByteCode = nullptr;

	std::vector<D3D12_INPUT_ELEMENT_DESC> mInputLayout;
private:
	ComPtr<ID3D12RootSignature> m_rootSignature = nullptr;
};


