#pragma once
#include "d3dUtil.h"
#include "WindowEngine.h"
#include "Component.h"
#include "Entity.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "entityManager.h"

class RenderEngine
{
public:
    RenderEngine();
    ~RenderEngine();

    bool Initialize(WindowEngine* wnd);

private:
    virtual void OnResize();
   

private:
    MeshRenderer* meshRenderer;
    WindowEngine* window;
    Shader* shader;
    Mesh* mesh;
    entityManager* g_EntityManager;

    //ComPtr<ID3D12RootSignature> mRootSignature = nullptr;
    ComPtr<ID3D12DescriptorHeap> mCbvHeap = nullptr;



};

