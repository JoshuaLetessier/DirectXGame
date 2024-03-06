#pragma once
#include "../DirectXMoteur/Entity.h"
#include "../DirectXMoteur/pch.h"

struct Cords
{
	float x, y, z;
};

class Ennemies
{
private:
	MeshRenderer ennemy;
	vector<Entity> m_Entity;

public:
	Ennemies();
	void spawnEnnemies();
	Cords spawnAleatoire();
	Cords offsetCalcul(int x, int y, int z);
	void deleteEnnemies(int i);
};

