#include "MeshRenderer.h"
#include "d3dUtil.h"
#include "DXParam.h"
#include "Window.h"
#include <iostream>

MeshRenderer::MeshRenderer()
{
}

bool MeshRenderer::Initialize(ComPtr<ID3D12GraphicsCommandList> m_CommandList, ComPtr<ID3D12CommandAllocator> m_CommandAllocators)
{
	DXParam dxParam;
	Window window;
	// Reset the command list to prep for initialization commands.

	//ComPtr<ID3D12GraphicsCommandList> m_CommandList = dxParam.g_CommandList;

	ThrowIfFailed(m_CommandList->Reset(m_CommandAllocators.Get(), nullptr));

	BuildDescriptorHeaps();
	BuildConstantBufferVertex();
	BuildRootSignature();
	BuildShader();
	InputElement();
	CreateCubeGeometry();
	BuildPSO();

	// Execute the initialization commands.
	ThrowIfFailed(dxParam.g_CommandList->Close());
	ID3D12CommandList* cmdsLists[] = { dxParam.g_CommandList.Get() };
	mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

	// Wait until initialization is complete.
	dxParam.Flush(dxParam.g_CommandQueue, dxParam.g_Fence, window.g_FenceValue, window.g_FenceEvent);

	return true;
}

void MeshRenderer::Update()
{
	XMMATRIX world = XMMatrixIdentity();
	XMMATRIX view = XMMatrixLookAtLH(XMVectorSet(0.0f, 0.0f, -5.0f, 0.0f), XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f), XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
	XMMATRIX proj = XMMatrixPerspectiveFovLH(0.25f * XM_PI, 800 / 600, 1.0f, 1000.0f);

	XMMATRIX worldProj = world * proj;

	worldProj = XMMatrixTranspose(worldProj);

	XMFLOAT4X4 worldProjFloat4x4;
	XMStoreFloat4x4(&worldProjFloat4x4, worldProj);
}

//void MeshRenderer::OnResize()
//{
//    window.Resize();
//
//    // The window resized, so update the aspect ratio and recompute the projection matrix.
//    XMMATRIX P = XMMatrixPerspectiveFovLH(0.25f * MathHelper::Pi, AspectRatio(), 1.0f, 1000.0f);
//    XMStoreFloat4x4(&mProj, P);
//}

void MeshRenderer::Draw()
{
	DXParam dxParam;
	Window window;

	CD3DX12_RESOURCE_BARRIER transition = CD3DX12_RESOURCE_BARRIER::Transition(dxParam.CurrentBackBuffer(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);

	ThrowIfFailed(dxParam.g_CommandAllocators->Reset());

	ThrowIfFailed(dxParam.g_CommandList->Reset(dxParam.g_CommandAllocators->Get(), dxParam.g_Pso.Get()));

	dxParam.g_CommandList->RSSetViewports(1, &mScreenViewport);
	dxParam.g_CommandList->RSSetScissorRects(1, &mScissorRect);

	dxParam.g_CommandList->ResourceBarrier(1, &transition);

	dxParam.g_CommandList->ClearRenderTargetView(dxParam.CurrentBackBufferView(), Colors::LightSteelBlue, 0, nullptr);
	dxParam.g_CommandList->ClearDepthStencilView(dxParam.DepthStencilView(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);

	D3D12_CPU_DESCRIPTOR_HANDLE currentBackBufferView = dxParam.CurrentBackBufferView();
	D3D12_CPU_DESCRIPTOR_HANDLE depthStencilView = dxParam.DepthStencilView();
	dxParam.g_CommandList->OMSetRenderTargets(1, &currentBackBufferView, true, &depthStencilView);

	ID3D12DescriptorHeap* descriptorHeaps[] = { mCbvHeap.Get() };
	dxParam.g_CommandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);
	dxParam.g_CommandList->SetGraphicsRootSignature(mRootSignature.Get());

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView = mCubeGeo->VertexBufferView();
	dxParam.g_CommandList->IASetVertexBuffers(0, 1, &vertexBufferView);

	D3D12_INDEX_BUFFER_VIEW indexBufferView = mCubeGeo->IndexBufferView();
	dxParam.g_CommandList->IASetIndexBuffer(&indexBufferView);
	dxParam.g_CommandList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	dxParam.g_CommandList->SetGraphicsRootDescriptorTable(0, mCbvHeap->GetGPUDescriptorHandleForHeapStart());

	dxParam.g_CommandList->DrawIndexedInstanced(mCubeGeo->DrawArgs["box"].IndexCount, 1, 0, 0, 0);

	dxParam.g_CommandList->ResourceBarrier(1, &transition);

	ThrowIfFailed(dxParam.g_CommandList->Close());

	ID3D12CommandList* cmdsLists[] = { dxParam.g_CommandList.Get() };
	mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

	ThrowIfFailed(dxParam.g_SwapChain->Present(0, 0));
	mCurrBackBuffer = (mCurrBackBuffer + 1) % SwapChainBufferCount;

	dxParam.Flush(dxParam.g_CommandQueue, dxParam.g_Fence, window.g_FenceValue, window.g_FenceEvent);
}

void MeshRenderer::BuildDescriptorHeaps()
{
	D3D12_DESCRIPTOR_HEAP_DESC cbvheapDesc;
	cbvheapDesc.NumDescriptors = 1;
	cbvheapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	cbvheapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	cbvheapDesc.NodeMask = 0;
	if (md3dDevice != NULL)
	{
		return;
	}
	else
	{
		printf("test %p\n", md3dDevice);
		ThrowIfFailed(md3dDevice->CreateDescriptorHeap(&cbvheapDesc, IID_PPV_ARGS(&mCbvHeap)));
	}
}

void MeshRenderer::BuildConstantBufferVertex()
{
	mConstantBuffer = std::make_unique<UploadBuffer<ModelViewProjectionConstantBuffer>>(md3dDevice.Get(), 1, true);//pourquoi on le fait pas dans le h?
	UINT mCBByteSize = d3dUtil::CalcConstantBufferByteSize(sizeof(ModelViewProjectionConstantBuffer));

	D3D12_GPU_VIRTUAL_ADDRESS cbAddress = mConstantBuffer->Resource()->GetGPUVirtualAddress();

	int boxCBufIndex = 0;
	cbAddress += boxCBufIndex * mCBByteSize;

	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc;
	cbvDesc.BufferLocation = cbAddress;
	cbvDesc.SizeInBytes = d3dUtil::CalcConstantBufferByteSize(sizeof(ModelViewProjectionConstantBuffer));

	md3dDevice->CreateConstantBufferView(&cbvDesc, mCbvHeap->GetCPUDescriptorHandleForHeapStart());
}

void MeshRenderer::BuildRootSignature()
{
	CD3DX12_ROOT_PARAMETER slotRootParem[1];

	CD3DX12_DESCRIPTOR_RANGE cbvTable;
	cbvTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);
	slotRootParem[0].InitAsDescriptorTable(1, &cbvTable);

	CD3DX12_ROOT_SIGNATURE_DESC rootSign(1, slotRootParem, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3D10Blob> serializedRootSig = nullptr;
	ComPtr<ID3D10Blob> errorBlob = nullptr;

	HRESULT hr = D3D12SerializeRootSignature(&rootSign, D3D_ROOT_SIGNATURE_VERSION_1_0, serializedRootSig.GetAddressOf(), errorBlob.GetAddressOf());
	if (errorBlob != nullptr)
		::OutputDebugStringA((char*)errorBlob->GetBufferPointer());//les deux point pour dire que c'est une fonction globale
	ThrowIfFailed(hr);

	//ThrowIfFailed(md3dDevice->CreateRootSignature(0, serializedRootSig->GetBufferPointer(), serializedRootSig->GetBufferSize(), IID_PPV_ARGS(&mRootSignature))); erreur � traiter
}

void MeshRenderer::BuildShader()
{
	HRESULT hr = S_OK;
	mvsByteCode = d3dUtil::CompileShader(L"Shader.hlsl", nullptr, "PS", "ps_5_0");
}

void MeshRenderer::InputElement()
{
	D3D12_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{"POSITION",0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
	};
}

void MeshRenderer::CreateCubeGeometry()
{
	DXParam dxParam;

	const UINT  sizeCubeMesh = sizeof(cubeMesh);
	const UINT  sizeIndicesCubeMesh = sizeof(m_cubeIndices);
	const UINT c_vertexBufferSize = sizeCubeMesh * sizeof(VertexPositionColor);
	const UINT c_indicesBufferSize = sizeIndicesCubeMesh * sizeof(std::uint16_t);

	mCubeGeo = std::make_unique<MeshGeometry>();
	mCubeGeo->Name = "cubeGeo";

	ThrowIfFailed(D3DCreateBlob(c_vertexBufferSize, &mCubeGeo->VertexBufferCPU));
	CopyMemory(mCubeGeo->VertexBufferCPU->GetBufferPointer(), cubeMesh.cubeVertices.data(), c_vertexBufferSize);

	ThrowIfFailed(D3DCreateBlob(c_indicesBufferSize, &mCubeGeo->IndexBufferCPU));
	CopyMemory(mCubeGeo->IndexBufferCPU->GetBufferPointer(), m_cubeIndices.data(), c_indicesBufferSize);

	mCubeGeo->VertexBufferGPU = d3dUtil::CreateDefaultBuffer(md3dDevice.Get(), dxParam.g_CommandList.Get(), cubeMesh.cubeVertices.data(), c_vertexBufferSize, mCubeGeo->VertexBufferUploader);//vertex
	mCubeGeo->IndexBufferGPU = d3dUtil::CreateDefaultBuffer(md3dDevice.Get(), dxParam.g_CommandList.Get(), m_cubeIndices.data(), c_indicesBufferSize, mCubeGeo->IndexBufferUploader);//index

	mCubeGeo->VertexByteStride = sizeof(VertexPositionColor);
	mCubeGeo->VertexBufferByteSize = c_vertexBufferSize;
	mCubeGeo->IndexFormat = DXGI_FORMAT_R16_UINT;
	mCubeGeo->IndexBufferByteSize = c_indicesBufferSize;

	SubmeshGeometry submesh;
	submesh.IndexCount = (UINT)sizeof(m_cubeIndices);
	submesh.StartIndexLocation = 0;
	submesh.BaseVertexLocation = 0;

	mCubeGeo->DrawArgs["box"] = submesh;
}

void MeshRenderer::BuildPSO()
{
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc;
	ZeroMemory(&psoDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	psoDesc.InputLayout = { mInputLayout.data(), (UINT)mInputLayout.size() };
	psoDesc.pRootSignature = mRootSignature.Get();
	psoDesc.VS =
	{
	 reinterpret_cast<BYTE*>(mvsByteCode->GetBufferPointer()),
	 mvsByteCode->GetBufferSize()
	};
	/* psoDesc.PS =
	 {
	  reinterpret_cast<BYTE*>(mpsByteCode->GetBufferPointer()),
	  mpsByteCode->GetBufferSize()
	 };*/
	psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	psoDesc.SampleMask = UINT_MAX;
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = mBackBufferFormat;
	psoDesc.SampleDesc.Count = m4xMsaaState ? 4 : 1;
	psoDesc.SampleDesc.Quality = m4xMsaaState ? (m4xMsaaQuality - 1) : 0;
	psoDesc.DSVFormat = mDepthStencilFormat;

	//ThrowIfFailed(md3dDevice->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&mPSO)));
}