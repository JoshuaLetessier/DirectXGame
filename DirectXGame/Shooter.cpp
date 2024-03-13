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

	if (!m_Entity.empty())
	{
		m_Entity[indice + 1].AddComponent(&balle);
		m_Entity[indice + 1].GetTransform().translate(0.0f, 0.0f, 0.02f);
		indice = m_Entity.size();
	}
	balle.Draw();
}

void Shooter::deleteBall(int i)
{
	m_Entity[i].~Entity();
}