#pragma once
#include <DirectXMath.h>

using namespace DirectX;

struct Transform {
	XMFLOAT3 vPos;
	XMFLOAT4X4 mPos;

	XMFLOAT3 vScaling;
	XMFLOAT4X4 mScaling;

	XMFLOAT3 vDir;
	XMFLOAT3 vRight;
	XMFLOAT3 vUp;
	XMFLOAT4X4 mRotate;
	XMFLOAT4 qRotate;

	XMFLOAT4X4 matrix;

	void identity();
};