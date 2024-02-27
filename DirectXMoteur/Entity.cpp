#include "Entity.h"

int Entity::nextId = 0;

Entity::Entity()
{
	m_id = nextId;
	nextId++;
}

Entity::~Entity()
{
}

void Entity::AddComponent(Component* component)
{
	m_components.push_back(component);
}

void Entity::GetComponent(Component* component)
{
	for (int i = 0; i < m_components.size(); i++)
	{
		if (m_components[i] == component)
		{
			return;
		}
	}
}

Transform& Entity::GetTransform()
{
	return m_transform;
}

int Entity::GetId() const
{
	return m_id;
}

void Entity::Update()
{
	for (int i = 0; i < m_components.size(); i++)
	{
		m_components[i]->Update();
	}
}