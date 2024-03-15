#pragma once

#include "WindowEngine.h"

#include "Entity.h"
#include "Component.h"
#include "Transform.h"

#include "d3dUtil.h"
#include "DirectXMath.h"


using namespace DirectX;

class WindowEngine;

class MeshRenderer : public Component
{
public:
    MeshRenderer();
    ~MeshRenderer();

    bool Initialize()override;
    void Update() override;

public:
    void Draw(Entity entity);
    void UpdateCamera();
    void BuildConstantBuffers();

    XMFLOAT4X4 mProj = MathHelper::Identity4x4();
public:

    XMFLOAT4X4 mWorld = MathHelper::Identity4x4();
    XMFLOAT4X4 mView = MathHelper::Identity4x4();
   
    float mTheta = 1.5f * XM_PI;
    float mPhi = XM_PIDIV4;
    float mRadius = 5.0f;

    UploadBuffer<Mesh::ModelViewProjectionConstantBuffer>* mObjectCB = nullptr;
    //UploadBuffer<Mesh::ModelViewProjectionConstantBuffer>* mObjectCBCamera = nullptr;

    Component* component;
    Transform* trans;
    WindowEngine* window;
    Shader* shader;
    Mesh* mesh;
};