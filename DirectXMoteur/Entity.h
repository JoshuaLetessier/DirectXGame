#pragma once
#include "pch.h"
#include "Transform.h"

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
	void GetComponent(Component* component);

	Transform& GetTransform();

	int GetId() const;

	void Update();
};