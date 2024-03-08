#pragma once
#include "pch.h"
#include <unordered_map>
#include "MathHelper.h"
#include "Component.h"

class Shader: public Component
{
public:
	struct ModelViewProjectionConstantBuffer
	{
		DirectX::XMFLOAT4X4 WorldViewProj = MathHelper::Identity4x4();
	};

	struct VertexPositionColor
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT4 color;
	};

	Shader();
	virtual ~Shader();

	bool Initialize(ID3D12Device* device, ID3D12GraphicsCommandList* commandList);
	void Update();
	
private:
	ComPtr<ID3D12RootSignature> m_rootSignature = nullptr;
};