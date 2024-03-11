#pragma once
#include "WindowEngine.h"
#include "Component.h"
#include "pch.h"
#include "Entity.h"

class Component;

class RenderEngine : public WindowEngine, public Component
{
public:
    RenderEngine(HINSTANCE hInstance);
    RenderEngine();
    ~RenderEngine();

    virtual bool Initialize()override;
    virtual void Draw()override;
private:
    virtual void OnResize()override;
    virtual void Update()override;
    void BuildDescriptorHeaps();
    void BuildConstantBuffers();
    void BuildRootSignature();
    void BuildPSO();

   

private:

    Mesh mesh;
    Shader shader;
    Entity entity;
   
    ComPtr<ID3D12RootSignature> mRootSignature = nullptr;
    ComPtr<ID3D12DescriptorHeap> mCbvHeap = nullptr;

    std::unique_ptr<UploadBuffer<Mesh::ModelViewProjectionConstantBuffer>> mObjectCB = nullptr;

    ComPtr<ID3D12PipelineState> mPSO = nullptr;

    XMFLOAT4X4 mWorld = MathHelper::Identity4x4();
    XMFLOAT4X4 mView = MathHelper::Identity4x4();
    XMFLOAT4X4 mProj = MathHelper::Identity4x4();

    float mTheta = 1.5f * XM_PI;
    float mPhi = XM_PIDIV4;
    float mRadius = 5.0f;
};

