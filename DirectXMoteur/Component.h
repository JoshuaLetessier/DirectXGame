#pragma once

class Component
{
public:
	Component() {}
	virtual ~Component() = default;

	virtual bool Initialize(ComPtr<ID3D12GraphicsCommandList> m_CommandList= nullptr, ComPtr<ID3D12CommandAllocator> m_CommandAllocators = nullptr) = 0;
	virtual void Update() = 0;
};