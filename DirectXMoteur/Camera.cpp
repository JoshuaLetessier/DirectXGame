#include "Camera.h"

Camera::Camera()
{
	SetLens(0.25f * MathHelper::Pi, 1.0f, 1.0f, 1000.0f);
}

Camera::~Camera()
{

}

bool Camera::Initialize()
{
	return true;
}

void Camera::Update()
{
	// Impl?mentation de la mise ? jour de la cam?ra
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
	DirectX::XMStoreFloat4x4(&mProj, P);
}

void Camera::Strafe(float d)
{
	// mPosition += d*mRight
	XMVECTOR s = DirectX::XMVectorReplicate(d);
	XMVECTOR r = DirectX::XMLoadFloat3(&trans.vRight);
	XMVECTOR p = DirectX::XMLoadFloat3(&trans.vPos);
	DirectX::XMStoreFloat3(&trans.vPos, DirectX::XMVectorMultiplyAdd(s, r, p));

	mViewDirty = true;
}

void Camera::Walk(float d)
{
	// mPosition += d*mLook
	XMVECTOR s = DirectX::XMVectorReplicate(d);
	XMVECTOR l = DirectX::XMLoadFloat3(&trans.vDir);
	XMVECTOR p = DirectX::XMLoadFloat3(&trans.vPos);
	DirectX::XMStoreFloat3(&trans.vPos, DirectX::XMVectorMultiplyAdd(s, l, p));

	mViewDirty = true;
}

void Camera::Pitch(float angle)
{
	// Rotate up and look vector about the right vector.

	XMMATRIX R = XMMatrixRotationAxis(DirectX::XMLoadFloat3(&trans.vRight), angle);

	DirectX::XMStoreFloat3(&trans.vUp, DirectX::XMVector3TransformNormal(DirectX::XMLoadFloat3(&trans.vUp), R));
	DirectX::XMStoreFloat3(&trans.vDir, DirectX::XMVector3TransformNormal(DirectX::XMLoadFloat3(&trans.vDir), R));

	mViewDirty = true;
}

void Camera::RotateY(float angle)
{
	// Rotate the basis vectors about the world y-axis.

	XMMATRIX R = DirectX::XMMatrixRotationY(angle);

	DirectX::XMStoreFloat3(&trans.vRight, DirectX::XMVector3TransformNormal(DirectX::XMLoadFloat3(&trans.vRight), R));
	DirectX::XMStoreFloat3(&trans.vUp, DirectX::XMVector3TransformNormal(DirectX::XMLoadFloat3(&trans.vUp), R));
	DirectX::XMStoreFloat3(&trans.vDir, DirectX::XMVector3TransformNormal(DirectX::XMLoadFloat3(&trans.vDir), R));

	mViewDirty = true;
}

DirectX::XMMATRIX Camera::GetView()const
{
	return DirectX::XMLoadFloat4x4(&trans.mView);
}

DirectX::XMMATRIX Camera::GetProj()const
{
	return DirectX::XMLoadFloat4x4(&trans.mProj);
}


DirectX::XMFLOAT4X4 Camera::GetView4x4f()const
{
	assert(!mViewDirty);
	return trans.mView;
}

DirectX::XMFLOAT4X4 Camera::GetProj4x4f()const
{
	return mProj;
}

void Camera::UpdateViewMatrix()
{
	if (mViewDirty)
	{
		XMVECTOR R = XMLoadFloat3(&trans.vRight);
		XMVECTOR U = XMLoadFloat3(&trans.vUp);
		XMVECTOR L = XMLoadFloat3(&trans.vDir);
		XMVECTOR P = XMLoadFloat3(&trans.vPos);

		// Keep camera's axes orthogonal to each other and of unit length.
		L = XMVector3Normalize(L);
		U = XMVector3Normalize(XMVector3Cross(L, R));

		// U, L already ortho-normal, so no need to normalize cross product.
		R = XMVector3Cross(U, L);

		// Fill in the view matrix entries.
		float x = -XMVectorGetX(XMVector3Dot(P, R));
		float y = -XMVectorGetX(XMVector3Dot(P, U));
		float z = -XMVectorGetX(XMVector3Dot(P, L));

		XMStoreFloat3(&trans.vRight, R);
		XMStoreFloat3(&trans.vUp, U);
		XMStoreFloat3(&trans.vDir, L);

		trans.mView(0, 0) = trans.vRight.x;
		trans.mView(1, 0) = trans.vRight.y;
		trans.mView(2, 0) = trans.vRight.z;
		trans.mView(3, 0) = x;

		trans.mView(0, 1) = trans.vUp.x;
		trans.mView(1, 1) = trans.vUp.y;
		trans.mView(2, 1) = trans.vUp.z;
		trans.mView(3, 1) = y;

		trans.mView(0, 2) = trans.vDir.x;
		trans.mView(1, 2) = trans.vDir.y;
		trans.mView(2, 2) = trans.vDir.z;
		trans.mView(3, 2) = z;

		trans.mView(0, 3) = 0.0f;
		trans.mView(1, 3) = 0.0f;
		trans.mView(2, 3) = 0.0f;
		trans.mView(3, 3) = 1.0f;

		mViewDirty = false;
	}
}