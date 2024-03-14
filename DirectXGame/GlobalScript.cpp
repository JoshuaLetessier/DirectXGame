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

void GlobalScript::start()
{
	
	Entity* ennemy = entityManage->createNewEntity();

	Ennemies* pScript = ennemy->addComponent<Ennemies>();
	//pScript->
}

void GlobalScript::update()
{
}
