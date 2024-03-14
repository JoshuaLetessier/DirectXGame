#include "Texture.h"

Texture2D::Texture2D()
{
}

Texture2D::~Texture2D()
{
}

void Texture2D::Initialize(ID3D12Device* device, ID3D12CommandQueue* commandQueue, ID3D12CommandAllocator* commandAllocator, ID3D12GraphicsCommandList* commandList, ID3D12DescriptorHeap* cbvHeap, ID3D12Fence* fence)
{
	m_pDevice = device;
	m_pCommandQueue = commandQueue;
	m_pDirectCmdListAlloc = commandAllocator;
	m_pCommandList = commandList;
	m_pCbvHeap = cbvHeap;
	m_pFence = fence;

	//create the root signature
	CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init(0, nullptr, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	HRESULT hr1 = (D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, signature.GetAddressOf(), error.GetAddressOf()));
	assert(hr1 == S_OK);

	HRESULT hr2 = (m_pDevice->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&m_pRootSignature)));
	assert(hr2 == S_OK);
}

void Texture2D::CreateTexture(const wchar_t* fileName)
{
	// reset command list
	m_pDirectCmdListAlloc->Reset();
	m_pCommandList->Reset(m_pDirectCmdListAlloc, nullptr);

	// init variables textures
	Microsoft::WRL::ComPtr<ID3D12Resource> texture;
	Microsoft::WRL::ComPtr<ID3D12Resource> textureUpload;

	// on load la texture ddsdepuis son emplacment de fichier
	ThrowIfFailed(CreateDDSTextureFromFile12(m_pDevice, m_pCommandList, fileName, texture, textureUpload));

	// créer un SRV Descriptors
	CD3DX12_CPU_DESCRIPTOR_HANDLE hDescriptor(m_pCbvHeap->GetCPUDescriptorHandleForHeapStart());

	// Pour trouver la taille d'une case du heap pour l'offset ici = 32
	//std::cout << "size d'une case du heap :" << m_pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) << std::endl;

	// On decale d'une case dans le heap ( une case = 32 octets )
	hDescriptor.Offset(m_textureIndex, m_pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV));
	m_textureIndex++;

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Format = texture->GetDesc().Format;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = texture->GetDesc().MipLevels;
	srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;
	m_pDevice->CreateShaderResourceView(texture.Get(), &srvDesc, hDescriptor);

	// close command list
	m_pCommandList->Close();

	// execute command list
	ID3D12CommandList* cmdsLists[] = { m_pCommandList };
	m_pCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

	// flush command list
	FlushCommandQueue();

	// get and need to store texture

	//ID3D12Resource* pTexture = texture.Get();
	m_Texture = texture.Get();

	texture.Detach();

	hDescriptorGPU = CD3DX12_GPU_DESCRIPTOR_HANDLE(m_pCbvHeap->GetGPUDescriptorHandleForHeapStart());
	hDescriptorGPU.Offset(m_textureIndex - 1, m_pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV));

	//return hDescriptorGPU;
}

void Texture2D::FlushCommandQueue()
{
	// Advance the fence value to mark commands up to this fence point.
	m_CurrentFence++;

	// Add an instruction to the command queue to set a new fence point.  Because we are on
	// the GPU timeline, the new fence point won't be set until the GPU finishes processing all
	// the commands prior to this Signal().
	HRESULT hrs = (m_pCommandQueue->Signal(m_pFence, m_CurrentFence));
	assert(hrs == S_OK);

	// Wait until the GPU has completed commands up to this fence point.
	if (m_pFence->GetCompletedValue() < m_CurrentFence)
	{
		HANDLE eventHandle = CreateEventEx(nullptr, nullptr, false, EVENT_ALL_ACCESS);

		// Fire event when GPU hits current fence.
		HRESULT hrd = (m_pFence->SetEventOnCompletion(m_CurrentFence, eventHandle));
		assert(hrd == S_OK);

		// Wait until the GPU hits current fence event is fired.
		if (eventHandle != 0)
		{
			WaitForSingleObject(eventHandle, INFINITE);
			CloseHandle(eventHandle);
		}
	}
}
