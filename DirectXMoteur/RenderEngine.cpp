#include "RenderEngine.h"
#include "iostream"



using Microsoft::WRL::ComPtr;


RenderEngine::RenderEngine(HINSTANCE hInstance) :WindowEngine(hInstance)
{
	
}

RenderEngine::RenderEngine() :Component()
{
	
}

RenderEngine::~RenderEngine()
{
	delete mObjectCBCamera;
	delete mObjectCB;
}

bool RenderEngine::Initialize()
{
	if (!WindowEngine::Initialize())
		return false;

	// Reset the command list to prep for initialization commands.
	ThrowIfFailed(mCommandList->Reset(mDirectCmdListAlloc.Get(), nullptr));

	BuildDescriptorHeaps();
	BuildConstantBuffers();
	BuildConstantBuffersCamera();
	shader.Initialize(md3dDevice);
	shader.BuildShadersAndInputLayout();
	mesh.Initialize(md3dDevice, mCommandList);
	shader.BuildPSO(md3dDevice, m4xMsaaState, m4xMsaaQuality);

	// Execute the initialization commands.
	ThrowIfFailed(mCommandList->Close());
	ID3D12CommandList* cmdsLists[] = { mCommandList.Get() };
	mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

	// Wait until initialization is complete.
	FlushCommandQueue();

	return true;
}

void RenderEngine::OnResize()
{
	WindowEngine::OnResize();

	// The window resized, so update the aspect ratio and recompute the projection matrix.
	XMMATRIX P = XMMatrixPerspectiveFovLH(0.25f * MathHelper::Pi, AspectRatio(), 1.0f, 1000.0f);
	XMStoreFloat4x4(&mProj, P);
}

void RenderEngine::Update()
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

void RenderEngine::UpdateCamera()
{
	// Convert Spherical to Cartesian coordinates.
	float x = mRadius * sinf(mPhi) * cosf(mTheta);
	float z = mRadius * sinf(mPhi) * sinf(mTheta);
	float y = mRadius * cosf(mPhi);

	// Build the view matrix.
	XMVECTOR pos = XMVectorSet(0, 0, 0, 0);
	XMVECTOR target = XMVectorSet(x, y, z, 1.0f);
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	XMMATRIX view = m_Camera->GetView();
	XMStoreFloat4x4(&trans.mView, view);

	XMMATRIX world = XMLoadFloat4x4(&trans.matrix);
	XMMATRIX proj = m_Camera->GetProj();

	wchar_t buffer[512];
	swprintf_s(buffer, L"world:\n%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n",
		world.r[0].m128_f32[0], world.r[0].m128_f32[1], world.r[0].m128_f32[2], world.r[0].m128_f32[3],
		world.r[1].m128_f32[0], world.r[1].m128_f32[1], world.r[1].m128_f32[2], world.r[1].m128_f32[3],
		world.r[2].m128_f32[0], world.r[2].m128_f32[1], world.r[2].m128_f32[2], world.r[2].m128_f32[3],
		world.r[3].m128_f32[0], world.r[3].m128_f32[1], world.r[3].m128_f32[2], world.r[3].m128_f32[3]);
	OutputDebugString(buffer);
	wchar_t buffer1[512];
	swprintf_s(buffer1, L"%s\nview:\n%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n",
		buffer,
		view.r[0].m128_f32[0], view.r[0].m128_f32[1], view.r[0].m128_f32[2], view.r[0].m128_f32[3],
		view.r[1].m128_f32[0], view.r[1].m128_f32[1], view.r[1].m128_f32[2], view.r[1].m128_f32[3],
		view.r[2].m128_f32[0], view.r[2].m128_f32[1], view.r[2].m128_f32[2], view.r[2].m128_f32[3],
		view.r[3].m128_f32[0], view.r[3].m128_f32[1], view.r[3].m128_f32[2], view.r[3].m128_f32[3]);
	OutputDebugString(buffer1);
	wchar_t buffer2[512];
	swprintf_s(buffer2, L"%s\nproj:\n%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n",
		buffer,
		proj.r[0].m128_f32[0], proj.r[0].m128_f32[1], proj.r[0].m128_f32[2], proj.r[0].m128_f32[3],
		proj.r[1].m128_f32[0], proj.r[1].m128_f32[1], proj.r[1].m128_f32[2], proj.r[1].m128_f32[3],
		proj.r[2].m128_f32[0], proj.r[2].m128_f32[1], proj.r[2].m128_f32[2], proj.r[2].m128_f32[3],
		proj.r[3].m128_f32[0], proj.r[3].m128_f32[1], proj.r[3].m128_f32[2], proj.r[3].m128_f32[3]);
	OutputDebugString(buffer2);

	XMMATRIX worldViewProj = world * view * proj;
	Mesh::ModelViewProjectionConstantBuffer mobjConstants;
	XMStoreFloat4x4(&mobjConstants.WorldViewProj, XMMatrixTranspose(worldViewProj));
	mObjectCB->CopyData(0, mobjConstants);
}

void RenderEngine::Draw()
{
	// Reuse the memory associated with command recording.
	// We can only reset when the associated command lists have finished execution on the GPU.
	ThrowIfFailed(mDirectCmdListAlloc->Reset());

	// A command list can be reset after it has been added to the command queue via ExecuteCommandList.
	// Reusing the command list reuses memory.
	ThrowIfFailed(mCommandList->Reset(mDirectCmdListAlloc.Get(), shader.mPSO.Get()));

	mCommandList->RSSetViewports(1, &mScreenViewport);
	mCommandList->RSSetScissorRects(1, &mScissorRect);

	// Indicate a state transition on the resource usage.
	CD3DX12_RESOURCE_BARRIER transition = CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
	mCommandList->ResourceBarrier(1, &transition);

	// Clear the back buffer and depth buffer.
	mCommandList->ClearRenderTargetView(CurrentBackBufferView(), Colors::LightSteelBlue, 0, nullptr);
	mCommandList->ClearDepthStencilView(DepthStencilView(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);

	// Specify the buffers we are going to render to.
	D3D12_CPU_DESCRIPTOR_HANDLE stockDepthStencilView = DepthStencilView();
	D3D12_CPU_DESCRIPTOR_HANDLE stockCurrentBackBufferView = CurrentBackBufferView();
	mCommandList->OMSetRenderTargets(1, &stockCurrentBackBufferView, true, &stockDepthStencilView);

	ID3D12DescriptorHeap* descriptorHeaps[] = { mCbvHeap.Get() };
	mCommandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);

	// per entity
	mCommandList->SetGraphicsRootSignature(shader.m_rootSignature.Get());

	D3D12_VERTEX_BUFFER_VIEW stockVertexBufferView = mesh.mBoxGeo->VertexBufferView();
	mCommandList->IASetVertexBuffers(0, 1, &stockVertexBufferView);

	D3D12_INDEX_BUFFER_VIEW stockIndexBufferView = mesh.mBoxGeo->IndexBufferView();
	mCommandList->IASetIndexBuffer(&stockIndexBufferView);
	mCommandList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//mCommandList->SetGraphicsRootDescriptorTable(0, mCbvHeap->GetGPUDescriptorHandleForHeapStart());
	mCommandList->SetGraphicsRootConstantBufferView(0, mObjectCB->Resource()->GetGPUVirtualAddress());

	mCommandList->DrawIndexedInstanced(mesh.mBoxGeo->DrawArgs["box"].IndexCount, 1, 0, 0, 0);
	// per entity

	// Indicate a state transition on the resource usage.
	CD3DX12_RESOURCE_BARRIER transition2 = CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
	mCommandList->ResourceBarrier(1, &transition2);

	// Done recording commands.
	ThrowIfFailed(mCommandList->Close());

	// Add the command list to the queue for execution.
	ID3D12CommandList* cmdsLists[] = { mCommandList.Get() };
	mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

	// swap the back and front buffers
	ThrowIfFailed(mSwapChain->Present(0, 0));
	mCurrBackBuffer = (mCurrBackBuffer + 1) % SwapChainBufferCount;

	// Wait until frame commands are complete.  This waiting is inefficient and is
	// done for simplicity.  Later we will show how to organize our rendering code
	// so we do not have to wait per frame.
	FlushCommandQueue();
}

void RenderEngine::BuildDescriptorHeaps()
{
	D3D12_DESCRIPTOR_HEAP_DESC cbvHeapDesc;
	cbvHeapDesc.NumDescriptors = 100;
	cbvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	cbvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	cbvHeapDesc.NodeMask = 0;
	ThrowIfFailed(md3dDevice->CreateDescriptorHeap(&cbvHeapDesc,
		IID_PPV_ARGS(&mCbvHeap)));
}

void RenderEngine::BuildConstantBuffers()
{
	mObjectCB = new UploadBuffer<Mesh::ModelViewProjectionConstantBuffer>(md3dDevice.Get(), 1, true);

	//UINT objCBByteSize = d3dUtil::CalcConstantBufferByteSize(sizeof(Mesh::ModelViewProjectionConstantBuffer));

	//D3D12_GPU_VIRTUAL_ADDRESS cbAddress = mObjectCB->Resource()->GetGPUVirtualAddress();
	//// Offset to the ith object constant buffer in the buffer.
	//int boxCBufIndex = 0;
	//cbAddress += boxCBufIndex * objCBByteSize;

	//D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc;
	//cbvDesc.BufferLocation = cbAddress;
	//cbvDesc.SizeInBytes = d3dUtil::CalcConstantBufferByteSize(sizeof(Mesh::ModelViewProjectionConstantBuffer));

	//md3dDevice->CreateConstantBufferView(
	//	&cbvDesc,
	//	mCbvHeap->GetCPUDescriptorHandleForHeapStart());
}

void RenderEngine::BuildConstantBuffersCamera()
{
	mObjectCBCamera = new UploadBuffer<Mesh::ModelViewProjectionConstantBuffer>(md3dDevice.Get(), 1, true);
}