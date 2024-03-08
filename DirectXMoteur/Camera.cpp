#include "Camera.h"

Camera::Camera()
{
	SetLens(0.25f * MathHelper::Pi, 1.0f, 1.0f, 1000.0f);
}

Camera::~Camera()
{
}

int Camera::getIdType() const
{
	return Id;
};

void Camera::SetLens(float fovY, float aspect, float zn, float zf)
{
	// cache properties
	mFovY = fovY;
	mAspect = aspect;
	mNearZ = zn;
	mFarZ = zf;

	mNearWindowHeight = 2.0f * mNearZ * tanf(0.5f * mFovY);
	mFarWindowHeight = 2.0f * mFarZ * tanf(0.5f * mFovY);

	DirectX::XMMATRIX P = DirectX::XMMatrixPerspectiveFovLH(mFovY, mAspect, mNearZ, mFarZ);
	XMStoreFloat4x4(&mProj, P);
}

void Camera::Strafe(float d)
{
	// mPosition += d*mRight
	XMVECTOR s = XMVectorReplicate(d);
	XMVECTOR r = XMLoadFloat3(&trans.vRight);
	XMVECTOR p = XMLoadFloat3(&trans.vPos);
	XMStoreFloat3(&trans.vPos, XMVectorMultiplyAdd(s, r, p));

	mViewDirty = true;
}

void Camera::Walk(float d)
{
	// mPosition += d*mLook
	XMVECTOR s = XMVectorReplicate(d);
	XMVECTOR l = XMLoadFloat3(&trans.vDir);
	XMVECTOR p = XMLoadFloat3(&trans.vPos);
	XMStoreFloat3(&trans.vPos, XMVectorMultiplyAdd(s, l, p));

	mViewDirty = true;
}

void Camera::Pitch(float angle)
{
	// Rotate up and look vector about the right vector.

	XMMATRIX R = XMMatrixRotationAxis(XMLoadFloat3(&trans.vRight), angle);

	XMStoreFloat3(&trans.vUp, XMVector3TransformNormal(XMLoadFloat3(&trans.vUp), R));
	XMStoreFloat3(&trans.vDir, XMVector3TransformNormal(XMLoadFloat3(&trans.vDir), R));

	mViewDirty = true;
}

void Camera::RotateY(float angle)
{
	// Rotate the basis vectors about the world y-axis.

	XMMATRIX R = XMMatrixRotationY(angle);

	XMStoreFloat3(&trans.vRight, XMVector3TransformNormal(XMLoadFloat3(&trans.vRight), R));
	XMStoreFloat3(&trans.vUp, XMVector3TransformNormal(XMLoadFloat3(&trans.vUp), R));
	XMStoreFloat3(&trans.vDir, XMVector3TransformNormal(XMLoadFloat3(&trans.vDir), R));

	mViewDirty = true;
}

DirectX::XMMATRIX Camera::GetView()const
{
	assert(!mViewDirty);
	return DirectX::XMLoadFloat4x4(&mProj);
}

DirectX::XMMATRIX Camera::GetProj()const
{
	return DirectX::XMLoadFloat4x4(&mProj);
}

DirectX::XMFLOAT4X4 Camera::GetView4x4f()const
{
	assert(!mViewDirty);
	return mProj;
}

DirectX::XMFLOAT4X4 Camera::GetProj4x4f()const
{
	return mProj;
}