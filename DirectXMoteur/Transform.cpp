#include "Transform.h"

void Transform::identity()
{
	//Initialisation des variables membres du struct
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

void Transform::rotate(float roll, float pitch, float yaw)
{
	XMVECTOR tempDir = XMLoadFloat3(&vDir);
	XMVECTOR tempRight = XMLoadFloat3(&vRight);
	XMVECTOR tempUp = XMLoadFloat3(&vUp);

	//Création de quaternion pour chaque rotation
	XMVECTOR quat = XMQuaternionRotationAxis(tempDir, roll);
	XMVECTOR tempQuat = quat;
	quat = XMQuaternionRotationAxis(tempRight, pitch);
	tempQuat = XMQuaternionMultiply(tempQuat, quat);
	quat = XMQuaternionRotationAxis(tempUp, yaw);
	tempQuat = XMQuaternionMultiply(tempQuat, quat);

	//Ajout rotation delta à l'objet
	XMVECTOR tempRotate = XMLoadFloat4(&qRotate);
	XMVECTOR tempRotateDelta = XMQuaternionMultiply(tempRotate, tempQuat);
	XMStoreFloat4(&qRotate, tempRotateDelta);

	//Convertir le quaternion en une matrice
	XMMATRIX matrixRot = XMMatrixRotationQuaternion(tempRotate);
	XMStoreFloat4x4(&mRotate, matrixRot);

	//Mise à jour des axes
	vRight.x = mRotate._11;
	vRight.y = mRotate._12;
	vRight.z = mRotate._13;

	vUp.x = mRotate._21;
	vUp.y = mRotate._22;
	vUp.z = mRotate._23;

	vDir.x = mRotate._31;
	vDir.y = mRotate._32;
	vDir.z = mRotate._33;
}

void Transform::update()
{
	//Met a jour la matrix global
	XMMATRIX mTempRotate = XMLoadFloat4x4(&mRotate);
	XMMATRIX mTempScaling = XMLoadFloat4x4(&mScaling);
	XMMATRIX mTempPos = XMLoadFloat4x4(&mPos);

	XMMATRIX mTempMatrix = mTempRotate;
	mTempMatrix *= mTempScaling;
	mTempMatrix *= mTempPos;

	XMStoreFloat4x4(&matrix, mTempMatrix);
}

void Transform::translate(float offsetX, float offsetY, float offsetZ)
{
	mPos._41 += offsetX;
	mPos._42 += offsetY;
	mPos._43 += offsetZ;
}