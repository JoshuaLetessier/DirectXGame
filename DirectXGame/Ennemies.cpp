//#include "Ennemies.h"
//
//Ennemies::Ennemies()
//{
//}
//
//void Ennemies::spawnEnnemies()
//{
//	Cords spawn = spawnAleatoire();
//	Cords offsetres = offsetCalcul(spawn.x, spawn.y, spawn.z);
//	for (int i = 0; i < sizeof(m_Entity); i++) {
//		m_Entity[i].AddComponent(&ennemy);
//		m_Entity[i].GetTransform().translate(offsetres.x, offsetres.y, offsetres.z);
//	}
//	ennemy.Draw();
//}
//
//Cords Ennemies::spawnAleatoire()
//{
//	float x = rand() % 200 + 500;
//	float y = rand() % 200 + 500;
//	float z = rand() % 200 + 500;
//
//	Cords nombre;
//	nombre.x = x;
//	nombre.y = y;
//	nombre.z = z;
//
//	return  nombre;
//}
//
//Cords Ennemies::offsetCalcul(int x, int y, int z)
//{
//	Cords offset;
//	offset.x = x / 100;
//	offset.y = y / 100;
//	offset.y = z / 100;
//
//	return offset;
//}
//
//void Ennemies::deleteEnnemies(int i)
//{
//	m_Entity[i].~Entity();
//}