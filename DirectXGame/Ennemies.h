#pragma once
#include "../DirectXMoteur/Entity.h"
#include "../DirectXMoteur/pch.h"
#include "../DirectXMoteur/entityManager.h"
#include "../DirectXMoteur/Component.h"
#include "../DirectXMoteur/MeshRenderer.h"
#include "../DirectXMoteur/ShaderStructure.h"

struct Cords
{
	float x, y, z;
};

class Ennemies
{
private:
	entityManager entityManager;

public:
	Ennemies();
	void spawnEnnemies();
	Cords spawnAleatoire();
	Cords offsetCalcul(float x, float y, float z);
	void deleteEnnemies(int i);

	

	Entity m_Entity;
	Component* m_Component;
	Mesh mesh;
	MeshRenderer meshRender;
	Shader shader;

};