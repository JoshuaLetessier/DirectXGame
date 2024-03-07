#include "Shooter.h"

Shooter::Shooter()
{

}

int Shooter::crossHair()
{
	
	int pointWidth;
	int pointHeight;

	pointWidth = window->mClientWidth / 2;
	pointHeight = window->mClientHeight / 2;

	return pointWidth, pointHeight;
}

void Shooter::shoot()
{
	int pointPos = Shooter::crossHair();

	for (int i = 0; i < sizeof(m_Entity); i++) {
		m_Entity[i].AddComponent(&balle);
		m_Entity[i].GetTransform().translate(0.0f, 0.0f, 0.02f);
	}
	balle.Draw();
}

void Shooter::deleteBall(int i)
{
	m_Entity[i].~Entity();
}