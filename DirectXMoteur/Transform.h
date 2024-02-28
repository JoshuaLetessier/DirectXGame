#pragma once
#include "pch.h"

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
	void rotate(float roll, float pitch, float yaw);
	void update();
};