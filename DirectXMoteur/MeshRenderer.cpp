#include "MeshRenderer.h"
#include "d3dUtil.h"

MeshRenderer::MeshRenderer()
{
    BuildDescriptorHeaps();
    BuildConstantBufferVertex();
    BuildRootSignature();
    BuildShader();
    InputElement();
    CreateCubeGeometry();
    BuildPSO();
}



void MeshRenderer::Draw()
{
    ThrowIfFailed(mDirectCmdListAlloc->Reset());

    ThrowIfFailed(mCommandList->Reset(mDirectCmdListAlloc.Get(), mPSO.Get()));

    mCommandList->RSSetViewports(1, &mScreenViewport);
    mCommandList->RSSetScissorRects(1, &mScissorRect);

    mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

    mCommandList->ClearRenderTargetView(CurrentBackBufferView(), Colors::LightSteelBlue, 0, nullptr);
    mCommandList->ClearDepthStencilView(DepthStencilView(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);

    mCommandList->OMSetRenderTargets(1, &CurrentBackBufferView(), true, &DepthStencilView());

    ID3D12DescriptorHeap* descriptorHeaps[] = { mCbvHeap.Get() };
    mCommandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);
    mCommandList->SetGraphicsRootSignature(mRootSignature.Get());

    mCommandList->IASetVertexBuffers(0, 1, &mCubeGeo->VertexBufferView());
    mCommandList->IASetIndexBuffer(&mCubeGeo->IndexBufferView());
    mCommandList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    mCommandList->SetGraphicsRootDescriptorTable(0, mCbvHeap->GetGPUDescriptorHandleForHeapStart());

    mCommandList->DrawIndexedInstanced(mCubeGeo->DrawArgs["box"].IndexCount, 1, 0, 0, 0);

    mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

    ThrowIfFailed(mCommandList->Close());

    ID3D12CommandList* cmdsLists[] = { mCommandList.Get() };
    mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

   
    ThrowIfFailed(mSwapChain->Present(0, 0));
    mCurrBackBuffer = (mCurrBackBuffer + 1) % SwapChainBufferCount;

    FlushCommandQueue();
}

void MeshRenderer::BuildDescriptorHeaps()
{
    D3D12_DESCRIPTOR_HEAP_DESC cbvheapDesc;
    cbvheapDesc.NumDescriptors = 1;
    cbvheapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    cbvheapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    cbvheapDesc.NodeMask = 0;

    ThrowIfFailed(md3dDevice->CreateDescriptorHeap(&cbvheapDesc, IID_PPV_ARGS(&mCbvHeap)));
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

    //ThrowIfFailed(md3dDevice->CreateRootSignature(0, serializedRootSig->GetBufferPointer(), serializedRootSig->GetBufferSize(), IID_PPV_ARGS(&mRootSignature))); erreur à traiter
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
	const UINT c_vertexBufferSize = sizeof(cubeMesh) * sizeof(VertexPositionColor);
	const UINT c_indicesBufferSize = sizeof(m_cubeIndices) * sizeof(std::uint16_t);

	mCubeGeo = std::make_unique<MeshGeometry>();
	mCubeGeo->Name = "cubeGeo";

    ThrowIfFailed(D3DCreateBlob(c_vertexBufferSize, &mCubeGeo->VertexBufferCPU));
    CopyMemory(mCubeGeo->VertexBufferCPU->GetBufferPointer(), cubeMesh.cubeVertices.data(), c_vertexBufferSize);

    ThrowIfFailed(D3DCreateBlob(c_indicesBufferSize, &mCubeGeo->IndexBufferCPU));
    CopyMemory(mCubeGeo->IndexBufferCPU->GetBufferPointer(), m_cubeIndices.data(), c_indicesBufferSize);

    mCubeGeo->VertexBufferGPU = d3dUtil:: CreateDefaultBuffer(md3dDevice.Get(), mCommandList.Get(), cubeMesh.cubeVertices.data(), c_vertexBufferSize, mCubeGeo->VertexBufferUploader);//vertex
    mCubeGeo->IndexBufferGPU = d3dUtil::CreateDefaultBuffer(md3dDevice.Get(), mCommandList.Get(), m_cubeIndices.data(), c_indicesBufferSize, mCubeGeo->IndexBufferUploader);//index

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