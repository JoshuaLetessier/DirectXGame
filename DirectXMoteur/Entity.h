#pragma once
#include "Transform.h"

#include "pch.h"

class Component;

class Entity
{
private:

	static int nextId;
	int m_id;
	std::vector<Component*> m_components;
	Transform m_transform;

public:
	Entity();
	virtual ~Entity();

	void AddComponent(Component* component);
	Component* GetComponent(Component* component);
	XMFLOAT4X4 addMatrix();

	Transform& GetTransform();

	int GetId() const;

	void Update();
};