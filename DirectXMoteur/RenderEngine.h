#pragma once
#include "d3dUtil.h"
#include "WindowEngine.h"
#include "Component.h"
#include "Entity.h"

//class Component;

class RenderEngine : public Component
{
public:
    RenderEngine(HINSTANCE hInstance);
    RenderEngine();
    ~RenderEngine();

    virtual bool Initialize();
    void Draw();
private:
    virtual void OnResize();
    void Update();
    void BuildDescriptorHeaps();
    void BuildConstantBuffers();
   

private:

    Mesh mesh;
    Shader shader;
    Entity entity;
    WindowEngine window;
    Component* box;
   
    ComPtr<ID3D12RootSignature> mRootSignature = nullptr;
    ComPtr<ID3D12DescriptorHeap> mCbvHeap = nullptr;

    std::unique_ptr<UploadBuffer<Mesh::ModelViewProjectionConstantBuffer>> mObjectCB = nullptr;

   

    XMFLOAT4X4 mWorld = MathHelper::Identity4x4();
    XMFLOAT4X4 mView = MathHelper::Identity4x4();
    XMFLOAT4X4 mProj = MathHelper::Identity4x4();

    float mTheta = 1.5f * XM_PI;
    float mPhi = XM_PIDIV4;
    float mRadius = 5.0f;
};

