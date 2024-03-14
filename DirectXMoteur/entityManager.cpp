#include "entityManager.h"


entityManager::entityManager() {};

entityManager::~entityManager() {};

Entity* entityManager::createNewEntity() {
	Entity* newEntity = new Entity();
	entityList.push_back(newEntity);
	return newEntity;
}
void entityManager::Update()
{
	for (int i = 0; i < entityList.size(); i++)
	{
		MeshRenderer* pMeshRender = (MeshRenderer*)entityList[i]->GetComponent("meshRenderer");
		if (entityList[i]->GetComponent("meshRenderer"))
		{
			pMeshRender->Update();
			pMeshRender->Draw(*entityList[i]);
		}
	}
}

