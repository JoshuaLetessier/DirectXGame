#include "Ennemies.h"

Ennemies::Ennemies()
{
}

void Ennemies::spawnEnnemies()
{
	Cords spawn = spawnAleatoire();
	entityManager.createNewEntity();
	Cords offsetres = offsetCalcul(spawn.x, spawn.y, spawn.z);

	
	entityManager.entityList.back()->AddComponent(&mesh, "mesh");
	entityManager.entityList.back()->AddComponent(&meshRender, "meshRender");
	entityManager.entityList.back()->AddComponent(&shader, "shader");

	entityManager.entityList.back()->Update();

	entityManager.entityList.back()->GetTransform()->translate(offsetres.x, offsetres.y, offsetres.z);//update
	
	//ennemy.Draw();
}

Cords Ennemies::spawnAleatoire()
{
	float x = static_cast<float>(rand() % 200 + 500);
	float y = static_cast<float>(rand() % 200 + 500);
	float z = static_cast<float>(rand() % 200 + 500);

	Cords nombre;
	nombre.x = x;
	nombre.y = y;
	nombre.z = z;

	return  nombre;
}

Cords Ennemies::offsetCalcul(float x, float y, float z)
{
	Cords offset;
	offset.x = x / 100;
	offset.y = y / 100;
	offset.y = z / 100;

	return offset;
}

void Ennemies::deleteEnnemies(int i)
{
	entityManager.entityList[i]->~Entity();
}

