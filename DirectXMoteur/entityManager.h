#pragma once
#include "pch.h"
#include "Entity.h"

class entityManager
{
public:
	entityManager();
	~entityManager();

	Entity* createNewEntity();

	vector<Entity*> entityList;
};