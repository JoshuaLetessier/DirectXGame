#pragma once
//#include "pch.h"
#include <DirectXMath.h>
using namespace DirectX;
class Transform {
public:

	Transform();
	~Transform(){}

	XMFLOAT3 vPos3;
	XMFLOAT4 vPos4;
	XMFLOAT4X4 mPos;

	XMFLOAT3 vScal3;
	XMFLOAT4 vScal4;
	XMFLOAT4X4 mScaling;

	XMFLOAT3 vDir /*= { 0.0f, 0.0f, 1.0f }*/;
	XMFLOAT3 vRight /*= { 1.0f, 0.0f, 0.0f }*/;
	XMFLOAT3 vUp /*= { 0.0f, 1.0f, 0.0f }*/;
	XMFLOAT4X4 mRotate;
	XMFLOAT4 qRotate;

	XMFLOAT4X4 matrix;

	void identity();
	void rotate(float roll, float pitch, float yaw);
	void update();
	void translate(float offsetX, float offsetY, float offsetZ);

	XMFLOAT4X4 mView;
	
	float mRadius = 5.0f;

	void rotateCamera(float mPhi, float mTheta);
};