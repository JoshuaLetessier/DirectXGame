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
	bool Initialize(ID3D12Device* device, ID3D12GraphicsCommandList* commandList);
	void Update();

private:
	ComPtr<ID3D12RootSignature> m_rootSignature = nullptr;
};


