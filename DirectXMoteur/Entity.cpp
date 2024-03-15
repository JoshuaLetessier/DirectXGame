#include "Entity.h"
#include <iostream>
#include <string>
#include "pch.h"

int Entity::nextId = 0;

Entity::Entity()
{
	m_id = nextId;
	nextId++;

	meshRenderer = new MeshRenderer();
}

Entity::~Entity()
{
	delete meshRenderer;
}

void Entity::AddComponent(Component* component, const char* type)
{
	component->SetComponent(this, type);
	m_components.push_back(component);
}
 
//fnc templates
Component* Entity::GetComponent(const char* type)
{
	for (int i = 1; i < m_components.size(); i++)
	{
		if (m_components[i]->m_type == type)
		{
			return m_components[i];
		}
	}
}

XMFLOAT4X4 Entity::addMatrix()
{
	return m_transform.matrix;
}

Transform* Entity::GetTransform()
{
	return &m_transform;
}



int Entity::GetId() const
{
	return m_id;
}

void Entity::Update(WindowEngine* wng)
{
	for (int i = 0; i < m_components.size(); i++)
	{
		meshRenderer->Draw(*this, wng );
	}
}