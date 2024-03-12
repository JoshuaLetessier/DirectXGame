#pragma once
#include "pch.h"

class Score
{
public:
	Score();
	virtual ~Score();

	void Initialize(ID3D12Device* device, ID3D12GraphicsCommandList* commandList, int screenWidth, int screenHeight);
	void SetScore(int score);
	void Render();

	ID3D12Device* m_device;
	ID3D12GraphicsCommandList* m_commandList;
	int m_screenWidth;
	int m_screenHeight;
	int m_score;

	ComPtr<ID3D12RootSignature> m_rootSignature;
};
