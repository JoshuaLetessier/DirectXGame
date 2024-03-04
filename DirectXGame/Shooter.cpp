#include "Shooter.h"

Shooter::Shooter()
{

}

int Shooter::crossHair()
{
	Window window;
	int pointWidth; 
	int pointHeight; 

	pointWidth = window.g_ClientWidth / 2;
	pointHeight = window.g_ClientHeight / 2;

	return pointWidth, pointHeight;
}

void Shooter::shoot()
{
	int pointPos = Shooter::crossHair();

	for (int i = 0;  i < sizeof (m_Entity); i++) {
		m_Entity[i].AddComponent(&balle);
		m_Entity[i].GetTransform().translate(0.0f, 0.0f, 0.02f);
	}
}

void Shooter::deleteBall(int i)
{
	m_Entity[i].~Entity();
}