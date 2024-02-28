#pragma once
#include "pch.h"

class Collider
{
private:
	XMFLOAT3 m_position;
	XMFLOAT3 m_size;

public:
	Collider();
	~Collider();

	void SetPosition(const XMFLOAT3& newPosition);
	const XMFLOAT3& GetPosition() const;
	void SetSize(const XMFLOAT3& newSize);
	const XMFLOAT3& GetSize() const;

	bool IsColliding(const XMFLOAT3& point) const;


};

