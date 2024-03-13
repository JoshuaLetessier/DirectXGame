#include "Transform.h"
#include "Camera.h"
Camera* m_Camera;

Transform::Transform()
{
	mView = MathHelper::Identity4x4();
}
void Transform::identity()
{
	XMStoreFloat4x4(&mScaling, XMMatrixIdentity());
	(vScal4.x, vScal4.y, vScal4.z, vScal4.w) = (0.f, 0.f, 0.f, 1.f);
	(vDir.x, vDir.y, vDir.z) = (1.f, 0.f, 0.f);
	(vRight.x, vRight.y, vRight.z) = (0.f, 1.f, 0.f);
	(vUp.x, vUp.y, vUp.z) = (0.f, 0.f, 1.f);
	(qRotate.x, qRotate.y, qRotate.z, qRotate.w) = (0.f, 0.f, 0.f, 1.f);
	XMStoreFloat4x4(&mRotate, XMMatrixIdentity());
	(vPos4.x, vPos4.y, vPos4.z, vPos4.w) = (0.f, 0.f, 0.f, 1.f);
	XMStoreFloat4x4(&mPos, XMMatrixIdentity());
	XMStoreFloat4x4(&matrix, XMMatrixIdentity());
}

void Transform::rotate(float roll, float pitch, float yaw)
{
	XMVECTOR quat;
	XMVECTOR rotQuat{};


	// Yaw
	XMVECTOR dir;
	dir = XMLoadFloat3(&vDir);
	quat = XMQuaternionRotationAxis(dir, yaw); // Retourne le quaternion selon un axe
	rotQuat = XMQuaternionMultiply(rotQuat, quat); //donne le quaternion après rotation

	// Pitch
	XMVECTOR right;
	right = XMLoadFloat3(&vRight);
	quat = XMQuaternionRotationAxis(right, pitch);
	rotQuat = XMQuaternionMultiply(rotQuat, quat);

	// Roll
	XMVECTOR up;
	up = XMLoadFloat3(&vUp);
	quat = XMQuaternionRotationAxis(up, roll);
	rotQuat = XMQuaternionMultiply(rotQuat, quat);

	// Creation de la matrice de rotation à l'aide des quaternions
	XMVECTOR qCurrentRot;
	qCurrentRot = XMLoadFloat4(&qRotate);
	rotQuat = XMQuaternionMultiply(rotQuat, qCurrentRot);

	XMMATRIX rotMat;
	rotMat = XMMatrixRotationQuaternion(rotQuat);
	// stockage des valeurs 
	XMVECTOR storage = XMVectorZero(); // vecteur avec des 0 tu connais ;)

	XMVectorSetX(storage, XMVectorGetX(rotMat.r[0]));
	XMVectorSetY(storage, XMVectorGetY(rotMat.r[0]));
	XMVectorSetZ(storage, XMVectorGetZ(rotMat.r[0]));
	XMStoreFloat3(&vRight, storage);

	XMVectorSetX(storage, XMVectorGetX(rotMat.r[1]));
	XMVectorSetY(storage, XMVectorGetY(rotMat.r[1]));
	XMVectorSetZ(storage, XMVectorGetZ(rotMat.r[1]));
	XMStoreFloat3(&vDir, storage);

	XMVectorSetX(storage, XMVectorGetX(rotMat.r[2]));
	XMVectorSetY(storage, XMVectorGetY(rotMat.r[2]));
	XMVectorSetZ(storage, XMVectorGetZ(rotMat.r[2]));
	XMStoreFloat3(&vUp, storage);

	XMStoreFloat4x4(&mRotate, rotMat);
}

void Transform::update()
{
	// Create scaling matrix
	XMMATRIX scaleMat = XMMatrixScaling(vScal4.x, vScal4.y, vScal4.z);

	// Create rotation matrix
	XMMATRIX rotationMat = XMMatrixRotationQuaternion(XMLoadFloat4(&qRotate));

	// Create translation matrix
	XMMATRIX positionMat = XMMatrixTranslation(vPos4.x, vPos4.y, vPos4.z);

	XMMATRIX transformer; // transforme la matrice avec son échelle et sa position
	transformer = scaleMat; // Donne le scaling de la matrice
	transformer *= rotationMat; // Multiplie la matrice pour avoir la rotation
	transformer *= positionMat; // Multiplie la matrice pour avoir la position
	XMStoreFloat4x4(&matrix, transformer);

}

void Transform::translate(float offsetX, float offsetY, float offsetZ)
{
	mPos._41 += offsetX;
	mPos._42 += offsetY;
	mPos._43 += offsetZ;
}

void Transform::rotateCamera(float mPhi, float mTheta)
{
	// Convertir les coordonnées sphériques en coordonnées cartésiennes
	float x = mRadius * sinf(mPhi) * cosf(mTheta);
	float z = mRadius * sinf(mPhi) * sinf(mTheta);
	float y = mRadius * cosf(mPhi);

	// Construire la matrice de vue
	XMVECTOR target = XMVectorSet(x, y, z, 1.0f);
	XMVECTOR pos = XMVectorZero();
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	XMMATRIX view = XMMatrixLookAtLH(pos, target, up);

	// Convertir la matrice XMMATRIX en XMFLOAT4X4
	XMFLOAT4X4 viewMatrix;
	XMStoreFloat4x4(&viewMatrix, view);
}