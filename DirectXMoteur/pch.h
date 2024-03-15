#pragma once

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"Winmm.lib")

// Windows Runtime Library. Needed for Microsoft::WRL::ComPtr<> template class.
#include <wrl.h>
using namespace Microsoft::WRL;

// DirectX 12 specific headers.
#include <dxgi.h>
#include <dxgi1_4.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>
#include <DirectXColors.h>
#include <DirectXMath.h>

// D3D12 extension library.
#include "d3dx12.h"
#include <d3d12.h>

// STL Headers
#include <algorithm>
#include <cassert>
#include <chrono>
#include <vector>
#include <pix.h>
#include <DirectXCollision.h>
#include <memory>


#include "ShaderStructure.h"
#include "Component.h"
#include "Mesh.h"
#include "UploadBuffer.h"
#include "Timer.h"




#if defined(_DEBUG)
#include <dxgidebug.h>
#endif

using namespace DirectX;
using namespace std;


/* The min / max macros conflict with like - named member functions.
	Only use std::min and std::max defined in <algorithm>.*/
#if defined(min)
#undef min
#endif

#if defined(max)
#undef max
#endif
