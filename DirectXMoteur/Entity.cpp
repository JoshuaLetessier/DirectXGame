#include "Entity.h"
#include <iostream>
#include <string>
#include "pch.h"

int Entity::nextId = 0;

Entity::Entity()
{
	m_id = nextId;
	nextId++;

	
}

Entity::~Entity()
{
}

void Entity::AddComponent(Component* component, std::string type)
{
	component->SetComponent(this, type);
	m_components.push_back(component);
}
 
//fnc templates
Component* Entity::GetComponent(std::string type)
{
	for (int i = 0; i < m_components.size(); i++)
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

void Entity::Update()
{
	for (int i = 0; i < m_components.size(); i++)
	{
		//m_components[i]->Update();
		// 
		//on met tout dedans
	}
}