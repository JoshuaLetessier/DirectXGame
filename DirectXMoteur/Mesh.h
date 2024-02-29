//#pragma once
//
//#include "pch.h"
//using namespace DirectX;
//
//
//namespace renderObject
//{
//	struct Cube
//	{
//		VertexPositionColor cubeVertices[8] =
//		{
//			{ XMFLOAT3(-0.5f, -0.5f, -0.5f), XMFLOAT3(.0f, 0.0f, 0.0f) },
//			{ XMFLOAT3(-0.5f, -0.5f,  0.5f), XMFLOAT3(0.0f, 0.0f, 1.0f) },
//			{ XMFLOAT3(-0.5f,  0.5f, -0.5f), XMFLOAT3(0.0f, 1.0f, 0.0f) },
//			{ XMFLOAT3(-0.5f,  0.5f,  0.5f), XMFLOAT3(0.0f, 1.0f, 1.0f) },
//			{ XMFLOAT3(0.5f, -0.5f, -0.5f), XMFLOAT3(1.0f, 0.0f, 0.0f) },
//			{ XMFLOAT3(0.5f, -0.5f,  0.5f), XMFLOAT3(1.0f, 0.0f, 1.0f) },
//			{ XMFLOAT3(0.5f,  0.5f, -0.5f), XMFLOAT3(1.0f, 1.0f, 0.0f) },
//			{ XMFLOAT3(0.5f,  0.5f,  0.5f), XMFLOAT3(1.0f, 1.0f, 1.0f) },
//		};
//	};
//
//	unsigned short cubeIndices[36] =
//	{
//			0, 2, 1, // -x
//			1, 2, 3,
//
//			4, 5, 6, // +x
//			5, 7, 6,
//
//			0, 1, 5, // -y
//			0, 5, 4,
//
//			2, 6, 7, // +y
//			2, 7, 3,
//
//			0, 4, 6, // -z
//			0, 6, 2,
//
//			1, 3, 7, // +z
//			1, 7, 5,
//	};
//
//	struct Shape
//	{
//		VertexPositionColor shapeVertices[8] =
//		{
//			{ XMFLOAT3(-1.0f, -0.5f, -0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f) },
//			{ XMFLOAT3(-1.0f, -0.5f,  0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f) },
//			{ XMFLOAT3(-1.0f,  0.5f, -0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f) },
//			{ XMFLOAT3(-1.0f,  0.5f,  0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f) },
//			{ XMFLOAT3(1.0f, -0.5f, -0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f) },
//			{ XMFLOAT3(1.0f, -0.5f,  0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f) },
//			{ XMFLOAT3(1.0f,  0.5f, -0.5f), XMFLOAT3(.0f, 1.0f, 0.0f) },
//			{ XMFLOAT3(1.0f,  0.5f,  0.5f), XMFLOAT3(1.0f, 1.0f, 1.0f) },
//		};
//	};
//
//	unsigned short shabeIndices[36] =
//	{
//			0, 2, 1, // -x
//			1, 2, 3,
//
//			4, 5, 6, // +x
//			5, 7, 6,
//
//			0, 1, 5, // -y
//			0, 5, 4,
//
//			2, 6, 7, // +y
//			2, 7, 3,
//
//			0, 4, 6, // -z
//			0, 6, 2,
//
//			1, 3, 7, // +z
//			1, 7, 5,
//	};
//
//	struct Pyramide
//	{
//		VertexPositionColor pyramidVertices[5] =
//		{
//			{ XMFLOAT3(0.0f, 0.5f, 0.0f), XMFLOAT3(1.0f, 0.0f, 0.0f) },
//			{ XMFLOAT3(-0.5f, -0.5f, -0.5f), XMFLOAT3(0.0f, 1.0f, 0.0f) },
//			{ XMFLOAT3(0.5f, -0.5f, -0.5f), XMFLOAT3(0.0f, 0.0f, 1.0f) },
//			{ XMFLOAT3(-0.5f, -0.5f, 0.5f), XMFLOAT3(1.0f, 1.0f, 0.0f) },
//			{ XMFLOAT3(0.5f, -0.5f, 0.5f), XMFLOAT3(1.0f, 1.0f, 1.0f) },
//
//		};
//
//	};
//
//	unsigned short pyramideIndices[18] =
//	{
//		0, 1, 2, // face avant
//		0, 2, 4, // face droite
//		0, 4, 3, // face arrière
//		0, 3, 1, // face gauche
//
//		//base
//		4,3,1,//inversé pour visibles du dessous
//		4,1,2,
//	};
//
//	const float X = 0.525731112119133606f;
//	const float Z = 0.850650808352039932f;
//	struct Icosaedre //boule pas plate
//	{
//		VertexPositionColor IcosaedreVertices[12] =
//		{
//
//
//			{ XMFLOAT3(0.0f, 0.5f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f) },
//			{ XMFLOAT3(0.0f, -0.5f, 0.0f), XMFLOAT3(0.0f, 0.0f, 1.0f) },
//			{ XMFLOAT3(X, 0.0f, 0.0f), XMFLOAT3(1.0f, 0.0f, 0.0f) },
//
//			{ XMFLOAT3(-X, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 1.0f) },
//			{ XMFLOAT3(0.0f, 0.0f, Z), XMFLOAT3(1.0f, 0.0f, 1.0f) },
//			{ XMFLOAT3(0.0f, 0.0f, -Z), XMFLOAT3(0.0f, 1.0f, 1.0f) },
//
//			{ XMFLOAT3(X, 0.0f, Z), XMFLOAT3(0.0f, 1.0f, 1.0f) },
//			{ XMFLOAT3(-X, 0.0f, Z), XMFLOAT3(1.0f, 0.0f, 1.0f) },
//			{ XMFLOAT3(X, 0.0f, -Z), XMFLOAT3(1.0f, 1.0f, 1.0f) },
//
//			{ XMFLOAT3(-X, 0.0f, -X), XMFLOAT3(1.0f, 1.0f, 1.0f) },
//			{ XMFLOAT3(-X, 0.0f, -X), XMFLOAT3(1.0f, 1.0f, 1.0f) },
//			{ XMFLOAT3(0.0f, -0.5f, Z), XMFLOAT3(1.0f, 1.0f, 1.0f) },
//		};
//	};
//
//	unsigned short icosaedreIndices[90] =
//	{
//
//		/*0, 1, 2,
//		0, 2, 4,
//		0, 4, 3,
//		0, 3, 1,*/
//
//		4, 2, 5,
//		2, 0, 5,
//		0, 3, 5,
//		3, 1, 5,
//
//		//4, 5, 6,
//		//2, 5, 6,
//		//0, 5, 6,
//		//3, 5, 6,
//		//1, 6, 2,
//		//1, 6, 3,
//		//1, 6, 0,
//
//		//7, 6, 4,
//		//7, 6, 2,
//		//7, 6, 0,
//		//7, 6, 3,
//		//7, 6, 1
//	};
//};