#pragma once
#include "Transform.h"
#include <iostream>
#include <string>
#include <vector>


class Component;

class Entity
{
private:

	static int nextId;
	int m_id;
	std::vector<Component*> m_components;
	Component* comp;
	Transform m_transform;

public:
	Entity();
	virtual ~Entity();

	void AddComponent(Component* component, std::string type);
	Component* GetComponent(std::string type);
	XMFLOAT4X4 addMatrix();

	Transform* GetTransform();

	int GetId() const;

	void Update();
};