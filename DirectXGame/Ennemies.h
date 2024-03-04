#pragma once
#include "../DirectXMoteur/Entity.h"

class Ennemies
{
private:
	MeshRenderer ennemy;
	std::vector<Entity> m_Entity;
public:
	Ennemies();
	void spawnEnnemies();
	std::vector<int> spawnAleatoire();
	std::vector<float> offsetCalcul(int x, int y, int z);
	void deleteEnnemies(int i);
};

