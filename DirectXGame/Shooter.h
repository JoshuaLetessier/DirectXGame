#pragma once
#include "../DirectXMoteur/Window.h"
#include "../DirectXMoteur/Entity.h"

class Shooter
{
private:
	std::vector<Entity> m_Entity;
	MeshRenderer balle;
public:

	Shooter();
	int crossHair();
	void shoot();
	void deleteBall(int i);
};

