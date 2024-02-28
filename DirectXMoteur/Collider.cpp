#include "Collider.h"

Collider::Collider()
{
}

Collider::~Collider()
{
}

void Collider::SetCollider(float x, float y, float width, float height)
{
	m_x = x;
	m_y = y;
	m_width = width;
	m_height = height;
}