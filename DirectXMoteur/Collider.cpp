#include "Collider.h"

OrientedEntityCollision::OrientedEntityCollision()
    : orientedBox1(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f)),
    orientedBox2(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f)) {}

bool OrientedEntityCollision::Intersects() const {
    return orientedBox1.Intersects(orientedBox2);
}
