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

	struct ScoreSprite {
		ComPtr<ID3D12Resource> texture;
		D3D12_GPU_DESCRIPTOR_HANDLE textureDescriptor;
		D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
	};

	ID3D12Device* m_device;
	ID3D12GraphicsCommandList* m_commandList;
	int m_screenWidth;
	int m_screenHeight;
	int m_score;

	ComPtr<ID3D12RootSignature> m_rootSignature;
	ComPtr<ID3D12PipelineState> m_pipelineState;
	std::vector<ScoreSprite> m_scoreSprites;
};
