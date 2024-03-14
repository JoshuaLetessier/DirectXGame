#include "MeshRenderer.h"

MeshRenderer::MeshRenderer()
{
}

MeshRenderer::~MeshRenderer()
{
	delete mObjectCBCamera;
	delete mObjectCB;

}

bool MeshRenderer::Initialize()
{
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

void MeshRenderer::Draw()
{
	// Reuse the memory associated with command recording.
	// We can only reset when the associated command lists have finished execution on the GPU.
	ThrowIfFailed(window->mDirectCmdListAlloc->Reset());

	// A command list can be reset after it has been added to the command queue via ExecuteCommandList.
	// Reusing the command list reuses memory.
	{
		HRESULT hr__ = (window->mCommandList->Reset(window->mDirectCmdListAlloc.Get(), shader->mPSO.Get())); std::wstring wfn = AnsiToWString("C:\\Users\\Faoll\\source\\repos\\DirectXGame\\DirectXMoteur\\MeshRenderer.cpp"); if ((((HRESULT)(hr__)) < 0)) {
			throw DxException(hr__, L"window->mCommandList->Reset(window->mDirectCmdListAlloc.Get(), shader->mPSO.Get())", wfn, 76);
		}
	};

	window->mCommandList->RSSetViewports(1, &window->mScreenViewport);
	window->mCommandList->RSSetScissorRects(1, &window->mScissorRect);

	// Indicate a state transition on the resource usage.
	CD3DX12_RESOURCE_BARRIER transition = CD3DX12_RESOURCE_BARRIER::Transition(window->CurrentBackBuffer(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
	window->mCommandList->ResourceBarrier(1, &transition);

	// Clear the back buffer and depth buffer.
	window->mCommandList->ClearRenderTargetView(window->CurrentBackBufferView(), Colors::LightSteelBlue, 0, nullptr);
	window->mCommandList->ClearDepthStencilView(window->DepthStencilView(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);

	// Specify the buffers we are going to render to.
	D3D12_CPU_DESCRIPTOR_HANDLE stockDepthStencilView = window->DepthStencilView();
	D3D12_CPU_DESCRIPTOR_HANDLE stockCurrentBackBufferView = window->CurrentBackBufferView();
	window->mCommandList->OMSetRenderTargets(1, &stockCurrentBackBufferView, true, &stockDepthStencilView);

	ID3D12DescriptorHeap* descriptorHeaps[] = { mCbvHeap.Get() };
	window->mCommandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);

	// per entity
	window->mCommandList->SetGraphicsRootSignature(shader->m_rootSignature);

	D3D12_VERTEX_BUFFER_VIEW stockVertexBufferView = mesh->mBoxGeo->VertexBufferView();
	window->mCommandList->IASetVertexBuffers(0, 1, &stockVertexBufferView);

	D3D12_INDEX_BUFFER_VIEW stockIndexBufferView = mesh->mBoxGeo->IndexBufferView();
	window->mCommandList->IASetIndexBuffer(&stockIndexBufferView);
	window->mCommandList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	window->mCommandList->SetGraphicsRootDescriptorTable(0, mCbvHeap->GetGPUDescriptorHandleForHeapStart());
	//mCommandList->SetGraphicsRootDescriptorTable(0, mCbvHeap->GetGPUDescriptorHandleForHeapStart());
	window->mCommandList->SetGraphicsRootConstantBufferView(0, mObjectCB->Resource()->GetGPUVirtualAddress());

	window->mCommandList->DrawIndexedInstanced(mesh->mBoxGeo->DrawArgs["box"].IndexCount, 1, 0, 0, 0);
	// per entity

	// Indicate a state transition on the resource usage.
	CD3DX12_RESOURCE_BARRIER transition2 = CD3DX12_RESOURCE_BARRIER::Transition(window->CurrentBackBuffer(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
	window->mCommandList->ResourceBarrier(1, &transition2);

	// Done recording commands.
	ThrowIfFailed(window->mCommandList->Close());

	// Add the command list to the queue for execution.
	ID3D12CommandList* cmdsLists[] = { window->mCommandList.Get() };
	window->mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

	// swap the back and front buffers
	ThrowIfFailed(window->mSwapChain->Present(0, 0));
	window->mCurrBackBuffer = (window->mCurrBackBuffer + 1) % window->SwapChainBufferCount;

	// Wait until frame commands are complete.  This waiting is inefficient and is
	// done for simplicity.  Later we will show how to organize our rendering code
	// so we do not have to wait per frame.
	window->FlushCommandQueue();
}

void MeshRenderer::BuildDescriptorHeaps(WindowEngine* wdw)
{
	window = wdw;
	D3D12_DESCRIPTOR_HEAP_DESC cbvHeapDesc;
	cbvHeapDesc.NumDescriptors = 100;
	cbvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	cbvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	cbvHeapDesc.NodeMask = 0;
	{
		HRESULT hr__ = (window->md3dDevice->CreateDescriptorHeap(&cbvHeapDesc, __uuidof(**(&mCbvHeap)), IID_PPV_ARGS_Helper(&mCbvHeap))); 
		std::wstring wfn = AnsiToWString("C:\\Users\\Faoll\\source\\repos\\DirectXGame\\DirectXMoteur\\MeshRenderer.cpp"); 
		if ((((HRESULT)(hr__)) < 0)) {
			throw DxException(hr__, L"window->md3dDevice->CreateDescriptorHeap(&cbvHeapDesc, IID_PPV_ARGS(&mCbvHeap))", wfn, 154);
		}
	};
}

void MeshRenderer::BuildConstantBuffers()
{
	mObjectCB = new UploadBuffer<Mesh::ModelViewProjectionConstantBuffer>(window->md3dDevice.Get(), 1, true);
}

void MeshRenderer::BuildConstantBuffersCamera()
{
	mObjectCBCamera = new UploadBuffer<Mesh::ModelViewProjectionConstantBuffer>(window->md3dDevice.Get(), 1, true);
}