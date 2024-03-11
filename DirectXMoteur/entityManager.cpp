#include "entityManager.h"

entityManager::entityManager() {};

entityManager::~entityManager() {};

Entity* entityManager::createNewEntity() {
	Entity* newEntity = new Entity();
	entityList.push_back(newEntity);
	return newEntity;
};
