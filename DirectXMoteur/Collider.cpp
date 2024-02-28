#include "Collider.h"

Collider::Collider()
{
	m_position = { 0.0f, 0.0f, 0.0f };
	m_size = { 1.0f, 1.0f, 1.0f };
}

Collider::~Collider()
{
}

void Collider:: SetPosition(const DirectX::XMFLOAT3& newPosition)
{
	m_position = newPosition;
}

void Collider::SetSize(const DirectX::XMFLOAT3& newSize)
{
	m_size = newSize;
}

const DirectX::XMFLOAT3& Collider::GetPosition() const
{
	return m_position;
}

const DirectX::XMFLOAT3& Collider::GetSize() const
{
	return m_size;
}

bool Collider::IsColliding(const DirectX::XMFLOAT3& point) const
{
	return (point.x >= m_position.x - m_size.x / 2 && point.x <=
		m_position.x + m_size.x / 2 &&
		point.y >= m_position.y - m_size.y / 2 && point.y <=
		m_position.y + m_size.y / 2 &&
		point.z >= m_position.z - m_size.z / 2 && point.z <=
		m_position.z + m_size.z / 2);
}

