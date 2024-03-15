#pragma once
#include "Transform.h"
#include <iostream>
#include <vector>
#include "MeshRenderer.h"
#include "WindowEngine.h"


class Component;
class MeshRenderer;
class WindowEngine;

class Entity
{
private:

	static int nextId;
	int m_id;
	std::vector<Component*> m_components;
	//Component* comp;
	Transform m_transform;
	MeshRenderer* meshRenderer;

public:
	Entity();
	virtual ~Entity();

	void AddComponent(Component* component, const char* type);
	Component* GetComponent(const char* type);
	XMFLOAT4X4 addMatrix();

	Transform* GetTransform();

	template <typename T>
	T* addComponent();

	int GetId() const;

	void Update(WindowEngine* wng);
};

template <typename T>
//Create script
T* Entity::addComponent()
{
	T* c = new T();
	m_components.push_back((Component*)c);
	return c;
}
