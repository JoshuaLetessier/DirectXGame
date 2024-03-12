#pragma once
#include "../DirectXMoteur/Entity.h"
#include "../DirectXMoteur/pch.h"
#include "../DirectXMoteur/RenderEngine.h"

struct Cords
{
	float x, y, z;
};

class Ennemies
{
private:
	RenderEngine ennemy;

public:
	Ennemies();
	void spawnEnnemies();
	Cords spawnAleatoire();
	Cords offsetCalcul(float x, float y, float z);
	void deleteEnnemies(int i);

	vector<Entity> m_Entity;
};