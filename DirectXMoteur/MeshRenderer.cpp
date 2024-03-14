#include "MeshRenderer.h"

MeshRenderer::MeshRenderer()
{
}

MeshRenderer::~MeshRenderer()
{
	//delete mObjectCBCamera;
	delete mObjectCB;

}

bool MeshRenderer::Initialize()
{
	BuildConstantBuffers();
	return false;
}

void MeshRenderer::Update()
{
	// Convert Spherical to Cartesian coordinates.
	float x = mRadius * sinf(mPhi) * cosf(mTheta);
	float z = mRadius * sinf(mPhi) * sinf(mTheta);
	float y = mRadius * cosf(mPhi);

	// Build the view matrix.
	XMVECTOR pos = XMVectorSet(x, y, z, 1.0f);
	XMVECTOR target = XMVectorZero();
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	XMMATRIX view = XMMatrixLookAtLH(pos, target, up);
	XMStoreFloat4x4(&mView, view);

	XMMATRIX world = XMLoadFloat4x4(&mWorld);
	XMMATRIX proj = XMLoadFloat4x4(&mProj);
	XMMATRIX worldViewProj = world * view * proj;

	// Update the constant buffer with the latest worldViewProj matrix.
	Mesh::ModelViewProjectionConstantBuffer objConstants;
	XMStoreFloat4x4(&objConstants.WorldViewProj, XMMatrixTranspose(worldViewProj));
	mObjectCB->CopyData(0, objConstants);
}

void MeshRenderer::UpdateCamera()
{
	// Convert Spherical to Cartesian coordinates.
	float x = mRadius * sinf(mPhi) * cosf(mTheta);
	float z = mRadius * sinf(mPhi) * sinf(mTheta);
	float y = mRadius * cosf(mPhi);

	// Build the view matrix.
	XMVECTOR pos = XMVectorSet(x, y, z, 1.0f);
	XMVECTOR target = XMVectorZero();
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	XMMATRIX view = XMMatrixLookAtLH(pos, target, up);
	XMStoreFloat4x4(&trans->mView, view);
	//XMMATRIX view = XMMatrixLookAtLH(pos, target, up);
	/*XMMATRIX view = cam.GetView();
	XMStoreFloat4x4(&mView, view);*/

	XMMATRIX world = XMLoadFloat4x4(&trans->matrix);
	XMMATRIX proj = window->m_Camera->GetProj();

	XMMATRIX worldViewProj = world * view * proj;


	// Update the constant buffer with the latest worldViewProj matrix.
	Mesh::ModelViewProjectionConstantBuffer mobjConstants;

	XMStoreFloat4x4(&mobjConstants.WorldViewProj, XMMatrixTranspose(worldViewProj));
	//std::unique_ptr<UploadBuffer<Mesh::ModelViewProjectionConstantBuffer>> mObjectCB;
	mObjectCB->CopyData(0, mobjConstants);
}

void MeshRenderer::Draw(Entity entity)
{
	Shader* pShader = (Shader*)entity.GetComponent("shader");
	window->mCommandList->SetGraphicsRootSignature(pShader->m_rootSignature);
	
	Mesh* pMesh = (Mesh*)entity.GetComponent("mesh");
	
	D3D12_VERTEX_BUFFER_VIEW stockVertexBufferView = pMesh->mBoxGeo->VertexBufferView();
	window->mCommandList->IASetVertexBuffers(0, 1, &stockVertexBufferView);
	

	D3D12_INDEX_BUFFER_VIEW stockIndexBufferView = pMesh->mBoxGeo->IndexBufferView();
	window->mCommandList->IASetIndexBuffer(&stockIndexBufferView);
	window->mCommandList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	window->mCommandList->SetGraphicsRootDescriptorTable(0, window->mCbvHeap->GetGPUDescriptorHandleForHeapStart());
	window->mCommandList->SetGraphicsRootConstantBufferView(0, mObjectCB->Resource()->GetGPUVirtualAddress());

	window->mCommandList->DrawIndexedInstanced(pMesh->mBoxGeo->DrawArgs["box"].IndexCount, 1, 0, 0, 0);
}



void MeshRenderer::BuildConstantBuffers()
{
	mObjectCB = new UploadBuffer<Mesh::ModelViewProjectionConstantBuffer>(window->md3dDevice.Get(), 1, true);
}

