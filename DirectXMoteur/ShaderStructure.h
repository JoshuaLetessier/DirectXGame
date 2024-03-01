#pragma once
#include "pch.h"
#include <string>
#include <unordered_map>



namespace shaderStruct
{
	
	struct ModelViewProjectionConstantBuffer
	{
		DirectX::XMFLOAT4X4 model;//matrice de transformation
		DirectX::XMFLOAT4X4 view;//matrice de vue, camera
		DirectX::XMFLOAT4X4 projection;//projection de la 3d sur la 2d
	};

	struct VertexPositionColor
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT4 color;
	};

	
}