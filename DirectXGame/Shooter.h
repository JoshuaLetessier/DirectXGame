#pragma once
#include "../DirectXMoteur/Entity.h"
#include "../DirectXMoteur/pch.h"
#include "../DirectXMoteur/entityManager.h"
#include "../DirectXMoteur/Component.h"
#include "../DirectXMoteur/MeshRenderer.h"
#include "../DirectXMoteur/ShaderStructure.h"

class Shooter
{
private:
	vector<Entity> m_Entity;
	int indice = 0;
public:
	WindowEngine* window;


	Shooter();
	int crossHair();
	void shoot();
	void deleteBall(int i);
};