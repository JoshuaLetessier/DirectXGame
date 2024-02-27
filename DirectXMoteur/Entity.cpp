#include "Entity.h"

int Entity::nextId = 0;

Entity::Entity()
{
	id = nextId;
	nextId++;
}

Entity::~Entity()
{
}

void Entity::AddComponent(Component* component)
{
	mcomponents.push_back(component);
}

void Entity::RemoveComponent(Component* component)
{
	for (int i = 0; i < mcomponents.size(); i++)
	{
		if (mcomponents[i] == component)
		{
			mcomponents.erase(mcomponents.begin() + i);
			break;
		}
	}
}

Transform& Entity::GetTransform()
{
	return mtransform;
}

int Entity::GetId() const
{
	return id;
}

void Entity::Update()
{
	for (int i = 0; i < mcomponents.size(); i++)
	{
		mcomponents[i]->Update();
	}
}