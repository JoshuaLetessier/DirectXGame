#include "Camera.h"

Camera::Camera()
{
}

Camera::~Camera()
{
}

Camera::Camera(Transform* transform) : m_Transform(*transform), m_Camera(this)
{
}

bool Camera::Initialize()
{
    // Mettez ici le code d'initialisation si nécessaire
    return true;
}

void Camera::Update()
{
    // Mettez ici le code de mise à jour si nécessaire
}

void Camera::Rotate(float roll, float pitch)
{
    m_Transform.rotateCamera(roll, pitch);
}

void Camera::SetViewMatrix(const XMFLOAT4X4& viewMatrix) {
    m_Transform.mView = viewMatrix;
}