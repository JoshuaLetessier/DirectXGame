#include "Ennemies.h"

Ennemies::Ennemies()
{
}

void Ennemies::spawnEnnemies()
{

	for (int i = 0; i < sizeof(m_Entity); i++) {
		m_Entity[i].AddComponent(&ennemy);
		m_Entity[i].GetTransform().translate();
	}
}

void Ennemies::deleteEnnemies(int i)
{
	m_Entity[i].~Entity();
}
