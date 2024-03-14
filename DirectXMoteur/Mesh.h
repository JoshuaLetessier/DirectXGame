#pragma once

#include <iostream>
#include "ShaderStructure.h"
#include "UploadBuffer.h"
#include <DirectXMath.h>

/*
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
*/

class Mesh: public Component
{
public:

	
	Mesh();
	virtual ~Mesh();

	//void BuildBoxGeometry();
	bool Initialize()override;
	bool Initialize(Microsoft::WRL::ComPtr<ID3D12Device> md3dDevice, Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> mCommandList);
	void Update();
	
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

	std::vector<VertexPositionColor> CubeVertices();
	std::vector<std::uint16_t> CubeIndices();
	std::unique_ptr<UploadBuffer<Mesh::ModelViewProjectionConstantBuffer>> UpdateBuffer(DirectX::XMMATRIX& world, DirectX::XMMATRIX& view, DirectX::XMMATRIX& proj);


public:
	std::unique_ptr<MeshGeometry> mBoxGeo = nullptr;	
};