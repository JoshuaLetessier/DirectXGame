#include "pch.h"
#include "Mesh.h"

Mesh::Mesh()
{

}

Mesh::~Mesh()
{
}

bool Mesh::Initialize()
{
	return false;
}

bool Mesh::Initialize(Microsoft::WRL::ComPtr<ID3D12Device> md3dDevice, Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> mCommandList)
{
	VertexPositionColor vertex;

	std::vector<VertexPositionColor> cubeVertices = CubeVertices();
	std::vector<std::uint16_t> cubeIndices = CubeIndices();

	const UINT vbByteSize = (UINT)cubeVertices.size() * sizeof(vertex);
	const UINT ibByteSize = (UINT)cubeIndices.size() * sizeof(std::uint16_t);

	mBoxGeo = std::unique_ptr<MeshGeometry>(new MeshGeometry());
	mBoxGeo->Name = "boxGeo";

	ThrowIfFailed(D3DCreateBlob(vbByteSize, &mBoxGeo->VertexBufferCPU));
	CopyMemory(mBoxGeo->VertexBufferCPU->GetBufferPointer(), cubeVertices.data(), vbByteSize);

	ThrowIfFailed(D3DCreateBlob(ibByteSize, &mBoxGeo->IndexBufferCPU));
	CopyMemory(mBoxGeo->IndexBufferCPU->GetBufferPointer(), cubeIndices.data(), ibByteSize);

	mBoxGeo->VertexBufferGPU = d3dUtil::CreateDefaultBuffer(md3dDevice.Get(),
		mCommandList.Get(), cubeVertices.data(), vbByteSize, mBoxGeo->VertexBufferUploader);

	mBoxGeo->IndexBufferGPU = d3dUtil::CreateDefaultBuffer(md3dDevice.Get(),
		mCommandList.Get(), cubeIndices.data(), ibByteSize, mBoxGeo->IndexBufferUploader);

	mBoxGeo->VertexByteStride = sizeof(vertex);
	mBoxGeo->VertexBufferByteSize = vbByteSize;
	mBoxGeo->IndexFormat = DXGI_FORMAT_R16_UINT;
	mBoxGeo->IndexBufferByteSize = ibByteSize;

	SubmeshGeometry submesh;
	submesh.IndexCount = (UINT)cubeIndices.size();
	submesh.StartIndexLocation = 0;
	submesh.BaseVertexLocation = 0;

	mBoxGeo->DrawArgs["box"] = submesh;

	return false;
}

void Mesh::Update()
{
	return;
}

std::vector<Mesh::VertexPositionColor> Mesh::CubeVertices()
{
	std::vector<VertexPositionColor> cubeVertices =
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
	return cubeVertices;
}

std::vector<std::uint16_t> Mesh::CubeIndices()
{
	std::vector<std::uint16_t> cubeIndices =
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
	return cubeIndices;
}

std::unique_ptr<UploadBuffer<Mesh::ModelViewProjectionConstantBuffer>> Mesh::UpdateBuffer(XMMATRIX &world, XMMATRIX &view, XMMATRIX &proj)
{
	world = XMMatrixTranspose(world);
	view = XMMatrixTranspose(view);
	proj = XMMatrixTranspose(proj);

	XMMATRIX worldViewProj = world * view * proj;


	// Update the constant buffer with the latest worldViewProj matrix.
	Mesh::ModelViewProjectionConstantBuffer objConstants;

	XMStoreFloat4x4(&objConstants.WorldViewProj, worldViewProj);
	std::unique_ptr<UploadBuffer<Mesh::ModelViewProjectionConstantBuffer>> mObjectCB;
	mObjectCB->CopyData(0, objConstants);

	return mObjectCB;
}