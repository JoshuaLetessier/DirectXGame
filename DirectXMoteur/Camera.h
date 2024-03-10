#pragma once
#include "Component.h"
#include "Transform.h"

class Camera : public Component
{
public:
    Camera();
    ~Camera();
    Camera(Transform* transform);

    virtual bool Initialize() override;
    virtual void Update() override;

    void Rotate(float roll, float pitch);

    void SetViewMatrix(const XMFLOAT4X4& viewMatrix);

private:
    Camera* m_Camera;
    Transform m_Transform;
};