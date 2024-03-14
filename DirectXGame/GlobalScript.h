#pragma once
#include "../DirectXMoteur/Script.h"
#include "../DirectXMoteur/entityManager.h"

class GlobalScript : public Script
{
public:
	GlobalScript();
	~GlobalScript();

	void start();
	void update();

	entityManager* entityManage;

};

