/*Gestion de l'affichage sur la fenetre*/

#include "RenderEngine.h"
#include "iostream"
#include "pch.h"

using namespace std;
using Microsoft::WRL::ComPtr;


RenderEngine::RenderEngine()
{
	g_EntityManager = new entityManager();
	shader = new Shader();
	mesh = new Mesh();
}

RenderEngine::~RenderEngine()
{
	delete g_EntityManager;
	delete shader;
	delete mesh;
}

bool RenderEngine::Initialize(WindowEngine* wnd)
{
	//window = wnd;	//Ne pas garder ?
	//meshRenderer = new MeshRenderer();
	//shader = new Shader();
	//mesh = new Mesh();
	
	// Reset the command list to prep for initialization commands.
	{
		HRESULT hr__ = (wnd->mCommandList->Reset(wnd->mDirectCmdListAlloc.Get(), nullptr)); std::wstring wfn = AnsiToWString("C:\\Users\\Faoll\\source\\repos\\DirectXGame\\DirectXMoteur\\RenderEngine.cpp"); if ((((HRESULT)(hr__)) < 0)) {
			throw DxException(hr__, L"wnd->mCommandList->Reset(wnd->mDirectCmdListAlloc.Get(), nullptr)", wfn, 27);
		}
	};


	
	//meshRenderer->BuildDescriptorHeaps(window);
	//meshRenderer->BuildConstantBuffers();
	shader->Initialize(wnd->md3dDevice.Get());
	shader->BuildShadersAndInputLayout();
	mesh->Initialize(wnd->md3dDevice, wnd->mCommandList);
	shader->BuildPSO(wnd->md3dDevice, wnd->m4xMsaaState, wnd->m4xMsaaQuality);



	// Execute the initialization commands.
	ThrowIfFailed(wnd->mCommandList->Close());
	ID3D12CommandList* cmdsLists[] = { wnd->mCommandList.Get() };
	wnd->mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

	// Wait until initialization is complete.
	wnd->FlushCommandQueue();

	return true;
}

void RenderEngine::OnResize()
{
	window->OnResize();

	// The window resized, so update the aspect ratio and recompute the projection matrix.
	XMMATRIX P = XMMatrixPerspectiveFovLH(0.25f * MathHelper::Pi, window->AspectRatio(), 1.0f, 1000.0f);
	XMStoreFloat4x4(&meshRenderer->mProj, P);
}

