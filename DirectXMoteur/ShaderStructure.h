#pragma once
#include "pch.h"
#include <string>
#include <unordered_map>
#include "MathHelper.h"



namespace shaderStruct
{
	
	struct ModelViewProjectionConstantBuffer
	{
		//DirectX::XMFLOAT4X4 model;//matrice de transformation
		//DirectX::XMFLOAT4X4 view;//matrice de vue, camera
		//DirectX::XMFLOAT4X4 projection;//projection de la 3d sur la 2d
		DirectX::XMFLOAT4X4 WorldViewProj = MathHelper::Identity4x4();
	};

	struct VertexPositionColor
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT4 color;
	};

	
}