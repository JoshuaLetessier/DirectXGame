#pragma once
#include "pch.h"
#include <unordered_map>
#include "MathHelper.h"

namespace shaderStruct
{
	struct ModelViewProjectionConstantBuffer
	{
		DirectX::XMFLOAT4X4 WorldViewProj = MathHelper::Identity4x4();
	};

	struct VertexPositionColor
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT4 color;
	};
}