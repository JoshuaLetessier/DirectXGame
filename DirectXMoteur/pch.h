#pragma once

#include <wrl.h>
#include <wrl/client.h>
#include <dxgi1_4.h>
#include <d3d12.h>
#include "d3dx12.h"
#include <DirectXColors.h>
#include <DirectXMath.h>

#include <pix.h>
#include <memory>
#include <vector>
//#include <agile.h>
#include <concrt.h>

#include "ShaderStructure.h"
#include "Component.h"
#include "Mesh.h"
#include "UploadBuffer.h"

#if defined(_DEBUG)
#include <dxgidebug.h>
#endif

using namespace DirectX;
using namespace shaderStruct;
using namespace renderObject;

Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> mCommandList;
Microsoft::WRL::ComPtr<ID3D12Device> md3dDevice;

D3D12_VIEWPORT mScreenViewport;
D3D12_RECT mScissorRect;

