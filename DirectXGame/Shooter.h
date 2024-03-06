#pragma once
#include "../DirectXMoteur/Window.h"
#include "../DirectXMoteur/Entity.h"
#include "../DirectXMoteur/pch.h"

class Shooter
{
private:
	vector<Entity> m_Entity;
	MeshRenderer balle;
public:

	Shooter();
	int crossHair();
	void shoot();
	void deleteBall(int i);
};
