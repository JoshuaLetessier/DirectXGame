#include "Transform.h"


void Transform::identity()
{
	vPos.x = 0.0f;
	vPos.y = 0.0f;
	vPos.z = 0.0f;

	mPos._11 = mPos._22 = mPos._33 = mPos._44 = 1.0f;
	mPos._12 = mPos._13 = mPos._14 = 0.0f;
	mPos._21 = mPos._23 = mPos._24 = 0.0f;
	mPos._31 = mPos._32 = mPos._34 = 0.0f;
	mPos._41 = mPos._42 = mPos._43 = 0.0f;

	vScaling.x = 1.0f;
	vScaling.y = 1.0f;
	vScaling.z = 1.0f;

	mScaling = mPos;

	vDir.x = 0.0f;
	vDir.y = 0.0f;
	vDir.z = 1.0f;

	vRight.x = 1.0f;
	vRight.y = 0.0f;
	vRight.z = 0.0f;

	vUp.x = 0.0f;
	vUp.y = 1.0f;
	vUp.z = 0.0f;

	mRotate = mPos;

	qRotate.x = 0.0f;
	qRotate.y = 0.0f;
	qRotate.z = 0.0f;
	qRotate.w = 1.0f;

	matrix = mPos;
}

void Transform::rotate()
{
	XMVECTOR quat;
	XMVECTOR tempQuat;
	XMVECTOR tempDir = XMLoadFloat3(&vDir);
	XMVECTOR tempRight = XMLoadFloat3(&vRight);
	XMVECTOR tempUp = XMLoadFloat3(&vUp);


	XMQuaternionRotationAxis(tempDir, roll);
	tempQuat = quat;
	XMQuaternionRotationAxis(tempRight, pitch);
	tempQuat *= quat;
	XMQuaternionRotationAxis(tempUp, yaw);
	tempQuat *= quat;

	XMStoreFloat4(&qRotate, tempQuat);


}

/*

 	XMMATRIX mTempRotate = XMLoadFloat4x4(&mRotate);
	XMMATRIX mTempScaling = XMLoadFloat4x4(&mScaling);
	XMMATRIX mTempPos = XMLoadFloat4x4(&mPos);

	XMMATRIX mTempMatrix = mTempRotate;
	mTempMatrix *= mTempScaling;
	mTempMatrix *= mTempPos;

	XMStoreFloat4x4(&matrix, mTempMatrix);
	*/