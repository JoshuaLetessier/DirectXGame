#pragma once
#include "pch.h"
#include "Entity.h"
#include "MeshRenderer.h"

class entityManager
{
public:
	entityManager();
	~entityManager();

	Entity* createNewEntity();
	//draw meshR
	//update list
	void Update();

	vector<Entity*> entityList;
};