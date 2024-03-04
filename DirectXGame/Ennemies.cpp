#include "Ennemies.h"

Ennemies::Ennemies()
{
}

void Ennemies::spawnEnnemies()
{
	std::vector<int> spawn = spawnAleatoire();
	std::vector<float> offset = offsetCalcul(spawn['x'], spawn['y'], spawn['z']);
	for (int i = 0; i < sizeof(m_Entity); i++) {
		m_Entity[i].AddComponent(&ennemy);
		m_Entity[i].GetTransform().translate(offset['x'], offset['y'], offset['z']);
	}
	ennemy.Draw();
}

std::vector<int> Ennemies::spawnAleatoire()
{
	float x = rand() % 200 + 500;
	float y = rand() % 200 + 500;
	float z = rand() % 200 + 500;
	
	std::vector<int> nombre{};
	nombre['x'] = x;
	nombre['y'] = y;
	nombre['z'] = z;

	return  nombre;
}

std::vector<float> Ennemies::offsetCalcul(int x, int y, int z)
{
	std::vector<float> offset{};
	offset['x'] = x / 100;
	offset['y'] = y / 100;
	offset['z'] = z / 100;

	return offset;
}

void Ennemies::deleteEnnemies(int i)
{
	m_Entity[i].~Entity();
}
