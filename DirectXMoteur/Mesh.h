#pragma once

#include "pch.h"

using namespace DirectX;

class Mesh: public Shader
{
public:
	struct Cube
	{
		vector<VertexPositionColor> cubeVertices =
		{
			VertexPositionColor({ XMFLOAT3(-0.5f, -0.5f, -0.5f), XMFLOAT4(.0f, 0.0f, 0.0f,1.0f) }),
			VertexPositionColor({ XMFLOAT3(-0.5f, -0.5f,  0.5f), XMFLOAT4(0.0f, 0.0f, 1.0f,1.0f) }),
			VertexPositionColor({ XMFLOAT3(-0.5f,  0.5f, -0.5f), XMFLOAT4(0.0f, 1.0f, 0.0f,1.0f) }),
			VertexPositionColor({ XMFLOAT3(-0.5f,  0.5f,  0.5f), XMFLOAT4(0.0f, 1.0f, 1.0f,1.0f) }),
			VertexPositionColor({ XMFLOAT3(0.5f, -0.5f, -0.5f), XMFLOAT4(1.0f, 0.0f, 0.0f,1.0f) }),
			VertexPositionColor({ XMFLOAT3(0.5f, -0.5f,  0.5f), XMFLOAT4(1.0f, 0.0f, 1.0f,1.0f) }),
			VertexPositionColor({ XMFLOAT3(0.5f,  0.5f, -0.5f), XMFLOAT4(1.0f, 1.0f, 0.0f,1.0f) }),
			VertexPositionColor({ XMFLOAT3(0.5f,  0.5f,  0.5f), XMFLOAT4(1.0f, 1.0f, 1.0f,1.0f) }),
		};
	};

	__declspec(selectany)vector<uint16_t> cubeIndices =
	{
			0, 2, 1, // -x
			1, 2, 3,

			4, 5, 6, // +x
			5, 7, 6,

			0, 1, 5, // -y
			0, 5, 4,

			2, 6, 7, // +y
			2, 7, 3,

			0, 4, 6, // -z
			0, 6, 2,

			1, 3, 7, // +z
			1, 7, 5,
	};

	Mesh();
	virtual ~Mesh();
};

//struct Shape
//{
//	VertexPositionColor shapeVertices[8] =
//	{
//		{ XMFLOAT3(-1.0f, -0.5f, -0.5f), XMFLOAT4(0.0f, 0.0f, 0.0f,1.0f) },
//		{ XMFLOAT3(-1.0f, -0.5f,  0.5f), XMFLOAT4(0.0f, 0.0f, 0.0f,1.0f) },
//		{ XMFLOAT3(-1.0f,  0.5f, -0.5f), XMFLOAT4(0.0f, 0.0f, 0.0f,1.0f) },
//		{ XMFLOAT3(-1.0f,  0.5f,  0.5f), XMFLOAT4(0.0f, 0.0f, 0.0f,1.0f) },
//		{ XMFLOAT3(1.0f, -0.5f, -0.5f), XMFLOAT4(0.0f, 0.0f, 0.0f,1.0f) },
//		{ XMFLOAT3(1.0f, -0.5f,  0.5f), XMFLOAT4(0.0f, 0.0f, 0.0f,1.0f) },
//		{ XMFLOAT3(1.0f,  0.5f, -0.5f), XMFLOAT4(.0f, 1.0f, 0.0f,1.0f) },
//		{ XMFLOAT3(1.0f,  0.5f,  0.5f), XMFLOAT4(1.0f, 1.0f, 1.0f,1.0f) },
//	};
//};

//unsigned short shapeIndices[36] =
//{
//		0, 2, 1, // -x
//		1, 2, 3,

//		4, 5, 6, // +x
//		5, 7, 6,

//		0, 1, 5, // -y
//		0, 5, 4,

//		2, 6, 7, // +y
//		2, 7, 3,

//		0, 4, 6, // -z
//		0, 6, 2,

//		1, 3, 7, // +z
//		1, 7, 5,
//};

//struct Pyramide
//{
//	VertexPositionColor pyramidVertices[5] =
//	{
//		{ XMFLOAT3(0.0f, 0.5f, 0.0f), XMFLOAT4(1.0f, 0.0f, 0.0f,1.0f) },
//		{ XMFLOAT3(-0.5f, -0.5f, -0.5f), XMFLOAT4(0.0f, 1.0f, 0.0f,1.0f) },
//		{ XMFLOAT3(0.5f, -0.5f, -0.5f), XMFLOAT4(0.0f, 0.0f, 1.0f,1.0f) },
//		{ XMFLOAT3(-0.5f, -0.5f, 0.5f), XMFLOAT4(1.0f, 1.0f, 0.0f,1.0f) },
//		{ XMFLOAT3(0.5f, -0.5f, 0.5f), XMFLOAT4(1.0f, 1.0f, 1.0f,1.0f) },

//	};

//};

//unsigned short pyramideIndices[18] =
//{
//	0, 1, 2, // face avant
//	0, 2, 4, // face droite
//	0, 4, 3, // face arrière
//	0, 3, 1, // face gauche

//	//base
//	4,3,1,//inversé pour visibles du dessous
//	4,1,2,
//};
