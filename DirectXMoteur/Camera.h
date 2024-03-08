#pragma once
#define CAMERA_H

#include "Component.h"
#include "Transform.h"
#include "MathHelper.h"

Transform trans;

class Camera : public Component
{
public:
    Camera();
    ~Camera();

    void SetLens(float fovY, float aspect, float zn, float zf);

    // Strafe/Walk the camera a distance d.
    void Strafe(float d);
    void Walk(float d);
    // Rotate the camera.
    void Pitch(float angle);
    void RotateY(float angle);

    int getIdType() const;

    DirectX::XMMATRIX GetView()const;
    DirectX::XMMATRIX GetProj()const;

    DirectX::XMFLOAT4X4 GetView4x4f()const;
    DirectX::XMFLOAT4X4 GetProj4x4f()const;
private:
    int Id = 1;
    float mNearZ = 0.0f;
    float mFarZ = 0.0f;
    float mAspect = 0.0f;
    float mFovY = 0.0f;
    float mNearWindowHeight = 0.0f;
    float mFarWindowHeight = 0.0f;

    bool mViewDirty = true;
    DirectX::XMFLOAT4X4 mProj = trans.matrix;
};