#pragma once
#include "GameTimer.h"
#include "../DirectXMoteur/d3d12.h"
#include "../DirectXMoteur/MathHelper.h"
#include "../DirectXMoteur/pch.h"
#include "../DirectXMoteur/InputManager.h"
#include "../DirectXMoteur/RenderEngine.h"
#include "Ennemies.h"

using namespace shaderStruct;


struct LunchGame
{
private:
    ComPtr<ID3D12RootSignature> mRootSignature = nullptr;
    ComPtr<ID3D12DescriptorHeap> mCbvHeap = nullptr;

    std::unique_ptr<UploadBuffer<ModelViewProjectionConstantBuffer>> mObjectCB = nullptr;

    std::unique_ptr<MeshGeometry> mBoxGeo = nullptr;

    ComPtr<ID3DBlob> mvsByteCode = nullptr;
    ComPtr<ID3DBlob> mpsByteCode = nullptr;

    std::vector<D3D12_INPUT_ELEMENT_DESC> mInputLayout;

    ComPtr<ID3D12PipelineState> mPSO = nullptr;

    XMFLOAT4X4 mWorld = MathHelper::Identity4x4();
    XMFLOAT4X4 mView = MathHelper::Identity4x4();
    XMFLOAT4X4 mProj = MathHelper::Identity4x4();

    float mTheta = 1.5f * XM_PI;
    float mPhi = XM_PIDIV4;
    float mRadius = 5.0f;

    GameTimer mTimer;
    Ennemies ennemy;
    InputManager input;

protected:
    wstring mMainWndCaption = L"d3d App";
    HWND    mhMainWnd = nullptr;
    bool      mAppPaused = false;

public:
	void update(const GameTimer& gt);
    int run();
    void CalculateFrameStats();
};

