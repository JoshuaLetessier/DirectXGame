#include "Score.h"

Score::Score()
{
}

Score::~Score()
{
}

void Score::Initialize(ID3D12Device* device, ID3D12GraphicsCommandList* commandList, int screenWidth, int screenHeight)
{
	m_device = device;
	m_commandList = commandList;
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	// Create the root signature
	{
		CD3DX12_DESCRIPTOR_RANGE textureSrvDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);
		CD3DX12_ROOT_PARAMETER rootParameters[1];
		rootParameters[0].InitAsDescriptorTable(1, &textureSrvDescriptorRange, D3D12_SHADER_VISIBILITY_PIXEL);

		CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
		rootSignatureDesc.Init(_countof(rootParameters), rootParameters, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

		ComPtr<ID3DBlob> signature;
		ComPtr<ID3DBlob> error;
		ThrowIfFailed(D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error));
		ThrowIfFailed(m_device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&m_rootSignature)));
	}

	// Create the pipeline state
	{
		ComPtr<ID3DBlob> vertexShader;
		ComPtr<ID3DBlob> pixelShader;

		/*UINT compileFlags = 0;*/
	}
}

void Score::SetScore(int score)
{
	m_score = score;
}

void Score::Render()
{

}