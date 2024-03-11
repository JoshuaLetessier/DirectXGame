#include "ShaderStructure.h"

Shader::Shader()
{
}

Shader::~Shader()
{
}

bool Shader::Initialize()
{
	return false;
}

bool Shader::Initialize(Microsoft::WRL::ComPtr<ID3D12Device> device)
{
	// Create the root signature
	{
		CD3DX12_DESCRIPTOR_RANGE range;
		CD3DX12_ROOT_PARAMETER parameter;
		CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;

		// Create a single descriptor table of CBVs
		range.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);
		parameter.InitAsDescriptorTable(1, &range, D3D12_SHADER_VISIBILITY_VERTEX);
		rootSignatureDesc.Init(1, &parameter, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

		ComPtr<ID3DBlob> signature;
		ComPtr<ID3DBlob> error;
		HRESULT hr = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error);
		if (error != nullptr)
		{
			::OutputDebugStringA((char*)error->GetBufferPointer());
		}
		ThrowIfFailed(hr);

		ThrowIfFailed(device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&m_rootSignature)));
	}

	{
		ComPtr<ID3DBlob> vertexShader;
		ComPtr<ID3DBlob> pixelShader;
		ComPtr<ID3DBlob> error;

		UINT compileFlags = 0;
	}

	return true;
}

void Shader::Update()
{
}

void Shader::BuildShadersAndInputLayout()
{
	HRESULT hr = S_OK;
	//mettre if debug enlever ../DirectXMoteur/ sinon le laisser
	mvsByteCode = d3dUtil::CompileShader(L"../DirectXMoteur/Shader.hlsl", nullptr, "VS", "vs_5_0");
	mpsByteCode = d3dUtil::CompileShader(L"../DirectXMoteur/Shader.hlsl", nullptr, "PS", "ps_5_0");

	mInputLayout =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};
}


