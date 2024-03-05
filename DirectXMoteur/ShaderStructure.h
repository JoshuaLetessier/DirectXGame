#pragma once
#include "pch.h"
#include <string>
#include <unordered_map>



namespace shaderStruct
{
	
	struct ModelViewProjectionConstantBuffer
	{
		XMFLOAT4X4 model;//matrice de transformation
		XMFLOAT4X4 view;//matrice de vue, camera
		XMFLOAT4X4 projection;//projection de la 3d sur la 2d
	};

	struct VertexPositionColor
	{
		XMFLOAT3 position;
		XMFLOAT4 color;
	};

	
}