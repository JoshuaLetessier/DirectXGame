#pragma once
#include "WindowEngine.h"
#include "Component.h"
#include "pch.h"
#include "Entity.h"
#include "Camera.h"
#include "Transform.h"

class Component;

class RenderEngine : public WindowEngine, public Component
{
public:
    RenderEngine(HINSTANCE hInstance);
    RenderEngine();
    ~RenderEngine();

    virtual bool Initialize()override;
    virtual void Draw()override;
    void UpdateCamera();
private:
    virtual void OnResize()override;
    virtual void Update()override;


    
    void BuildDescriptorHeaps();
    void BuildConstantBuffers();
    void BuildConstantBuffersCamera();
   
private:

    Mesh mesh;
    Shader shader;
    Entity entity;
    Transform trans;
   
    ComPtr<ID3D12RootSignature> mRootSignature = nullptr;
    ComPtr<ID3D12DescriptorHeap> mCbvHeap = nullptr;

    UploadBuffer<Mesh::ModelViewProjectionConstantBuffer>*mObjectCB = nullptr;
    UploadBuffer<Mesh::ModelViewProjectionConstantBuffer>* mObjectCBCamera = nullptr;
   
    XMFLOAT4X4 mWorld = MathHelper::Identity4x4();
    XMFLOAT4X4 mView = MathHelper::Identity4x4();
    XMFLOAT4X4 mProj = MathHelper::Identity4x4();

    int x, y, z;
    float mTheta = 1.5f * XM_PI;
    float mPhi = XM_PIDIV4;
    float mRadius = 5.0f;
};

