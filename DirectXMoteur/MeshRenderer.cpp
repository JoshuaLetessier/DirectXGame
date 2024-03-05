#include "MeshRenderer.h"
#include "d3dUtil.h"
#include "Window.h"
#include <iostream>


Window window;
ComPtr<ID3D12Device2> m_Device = window.g_Device;

MeshRenderer::MeshRenderer()
{
}

bool MeshRenderer::Initialize(ComPtr<ID3D12GraphicsCommandList> m_CommandList, Microsoft::WRL::ComPtr<ID3D12CommandAllocator> m_CommandAllocators)
{
    // Reset the command list to prep for initialization commands.
    
     ThrowIfFailed(m_CommandList->Reset(m_CommandAllocators.Get(), nullptr));
  
   // BuildDescriptorHeaps();
    //BuildConstantBufferVertex();
    BuildRootSignature();
    BuildShader();
    InputElement();
    CreateCubeGeometry();
    BuildPSO();

    // Execute the initialization commands.
    
    ThrowIfFailed(stockCommandList->Close());
    ID3D12CommandList* cmdsLists[] = { stockCommandList.Get() };
    stockCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

    // Wait until initialization is complete.
    window.Flush(stockCommandQueue, stockFence, window.g_FenceValue, window.g_FenceEvent);

    return true;
}

//void MeshRenderer::OnResize()
//{
//    window.Resize();
//
//    // The window resized, so update the aspect ratio and recompute the projection matrix.
//    XMMATRIX P = XMMatrixPerspectiveFovLH(0.25f * MathHelper::Pi, AspectRatio(), 1.0f, 1000.0f);
//    XMStoreFloat4x4(&mProj, P);
//}




void MeshRenderer::Draw(Microsoft::WRL::ComPtr<ID3D12CommandAllocator> m_CommandAllocators)
{    
    Window window;

    ThrowIfFailed(stockCommandList->Reset(m_CommandAllocators.Get(), window.g_Pso.Get()));
      
    stockCommandList->RSSetViewports(1, &mScreenViewport);
    stockCommandList->RSSetScissorRects(1, &mScissorRect);

    CD3DX12_RESOURCE_BARRIER transition = CD3DX12_RESOURCE_BARRIER::Transition(stockBackBuffers->Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
    printf("transition %p\n", transition);
    stockCommandList->ResourceBarrier(1, &transition);

    printf("test");
    stockCommandList->ClearRenderTargetView(window.CurrentBackBufferView(), Colors::LightSteelBlue, 0, nullptr);


    stockCommandList->ClearDepthStencilView(window.DepthStencilView(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);

    D3D12_CPU_DESCRIPTOR_HANDLE currentBackBufferView = window.CurrentBackBufferView();
    D3D12_CPU_DESCRIPTOR_HANDLE depthStencilView = window.DepthStencilView();
    stockCommandList->OMSetRenderTargets(1, &currentBackBufferView, true, &depthStencilView);


    ID3D12DescriptorHeap* descriptorHeaps[] = { mCbvHeap.Get() };
    stockCommandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);
    stockCommandList->SetGraphicsRootSignature(mRootSignature.Get());

    D3D12_VERTEX_BUFFER_VIEW vertexBufferView = mCubeGeo->VertexBufferView();
    stockCommandList->IASetVertexBuffers(0, 1, &vertexBufferView);

    D3D12_INDEX_BUFFER_VIEW indexBufferView = mCubeGeo->IndexBufferView();
    stockCommandList->IASetIndexBuffer(&indexBufferView);
    stockCommandList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    stockCommandList->SetGraphicsRootDescriptorTable(0, mCbvHeap->GetGPUDescriptorHandleForHeapStart());

    stockCommandList->DrawIndexedInstanced(mCubeGeo->DrawArgs["box"].IndexCount, 1, 0, 0, 0);


    stockCommandList->ResourceBarrier(1, &transition);

    ThrowIfFailed(stockCommandList->Close());

    ID3D12CommandList* cmdsLists[] = { stockCommandList.Get() };
    stockCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);


    ThrowIfFailed(window.g_SwapChain->Present(0, 0));
    mCurrBackBuffer = (mCurrBackBuffer + 1) % SwapChainBufferCount;

    
    window.Flush(stockCommandQueue, stockFence, window.g_FenceValue, window.g_FenceEvent);
}

void MeshRenderer::BuildDescriptorHeaps()
{
    D3D12_DESCRIPTOR_HEAP_DESC cbvheapDesc;
    cbvheapDesc.NumDescriptors = 1;
    cbvheapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    cbvheapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    cbvheapDesc.NodeMask = 0;

    window.CreateDescriptorHeap(m_Device ,cbvheapDesc.Type, cbvheapDesc.NumDescriptors);
}

void MeshRenderer::BuildConstantBufferVertex()
{
    printf("md3D %p\n", &m_Device);
    mConstantBuffer = std::make_unique<UploadBuffer<ModelViewProjectionConstantBuffer>>(m_Device.Get(), 1, true);
    UINT mCBByteSize = d3dUtil::CalcConstantBufferByteSize(sizeof(ModelViewProjectionConstantBuffer));

    D3D12_GPU_VIRTUAL_ADDRESS cbAddress = mConstantBuffer->Resource()->GetGPUVirtualAddress();

    int boxCBufIndex = 0;
    cbAddress += boxCBufIndex * static_cast<unsigned long long>(mCBByteSize);

    D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc;
    cbvDesc.BufferLocation = cbAddress;
    cbvDesc.SizeInBytes = d3dUtil::CalcConstantBufferByteSize(sizeof(ModelViewProjectionConstantBuffer));

    stockDevice->CreateConstantBufferView(&cbvDesc, mCbvHeap->GetCPUDescriptorHandleForHeapStart());
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
    //HRESULT hr = S_OK;
    mvsByteCode = d3dUtil::CompileShader(L"Shader.hlsl", nullptr, "VS", "vs_5_0");
    mpsByteCode = d3dUtil::CompileShader(L"Shader.hlsl", nullptr, "PS", "ps_5_0");
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

    mCubeGeo->VertexBufferGPU = d3dUtil:: CreateDefaultBuffer(m_Device.Get(), window.g_CommandList.Get(), cubeMesh.cubeVertices.data(), c_vertexBufferSize, mCubeGeo->VertexBufferUploader);//vertex
    mCubeGeo->IndexBufferGPU = d3dUtil::CreateDefaultBuffer(window.g_Device.Get(), window.g_CommandList.Get(), m_cubeIndices.data(), c_indicesBufferSize, mCubeGeo->IndexBufferUploader);//index

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