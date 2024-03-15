#include "GlobalScript.h"
#include "Ennemies.h"

GlobalScript::GlobalScript()
{
	entityManage = new entityManager();
}

GlobalScript::~GlobalScript()
{
	delete entityManage;
}

void GlobalScript::start(WindowEngine* wng)
{
	
	Entity* ennemy = entityManage->createNewEntity();

	Ennemies* pScript = ennemy->addComponent<Ennemies>();
	pScript->spawnEnnemies(ennemy, wng);
}

void GlobalScript::update()
{
}
