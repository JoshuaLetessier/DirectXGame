#pragma once
#include "../DirectXMoteur/WindowEngine.h"
#include "../DirectXMoteur/RenderEngine.h"
#include "../DirectXMoteur/Entity.h"


class Shooter
{
private:
	vector<Entity> m_Entity;
	RenderEngine balle;
	int indice = 0;
public:
	WindowEngine* window;


	Shooter();
	int crossHair();
	void shoot();
	void deleteBall(int i);
};