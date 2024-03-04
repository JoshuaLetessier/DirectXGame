#pragma once
#include "../DirectXMoteur/Entity.h"

class Ennemies
{
private:
	Component ennemy;
	std::vector<Entity> m_Entity;
public:
	Ennemies();
	void spawnEnnemies();
	void deleteEnnemies(int i);
};

