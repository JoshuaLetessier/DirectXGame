/*Gestion de l'affichage sur la fenetre*/

#include "RenderEngine.h"
#include "iostream"
#include "pch.h"

using namespace std;
using Microsoft::WRL::ComPtr;


RenderEngine::RenderEngine()
{
}

RenderEngine::~RenderEngine()
{
	
}

bool RenderEngine::Initialize(WindowEngine* wnd)
{
	window = wnd;

	if (!window->Initialize())
		return false;

	// Reset the command list to prep for initialization commands.
	ThrowIfFailed(window->mCommandList->Reset(window->mDirectCmdListAlloc.Get(), nullptr));

	meshRenderer->BuildDescriptorHeaps();
	meshRenderer->BuildConstantBuffers();
	shader->Initialize(window->md3dDevice);
	meshRenderer->BuildConstantBuffersCamera();
	shader->Initialize(window->md3dDevice);
	shader->BuildShadersAndInputLayout();
	mesh->Initialize(window->md3dDevice, window->mCommandList);
	shader->BuildPSO(window->md3dDevice, window->m4xMsaaState, window->m4xMsaaQuality);

	// Execute the initialization commands.
	ThrowIfFailed(window->mCommandList->Close());
	ID3D12CommandList* cmdsLists[] = { window->mCommandList.Get() };
	window->mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

	// Wait until initialization is complete.
	window->FlushCommandQueue();

	return true;
}

void RenderEngine::OnResize()
{
	window->OnResize();

	// The window resized, so update the aspect ratio and recompute the projection matrix.
	XMMATRIX P = XMMatrixPerspectiveFovLH(0.25f * MathHelper::Pi, window->AspectRatio(), 1.0f, 1000.0f);
	XMStoreFloat4x4(&meshRenderer->mProj, P);
}

