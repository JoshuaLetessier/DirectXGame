#pragma once
#include "../DirectXMoteur/Script.h"
#include "../DirectXMoteur/entityManager.h"

#include "../DirectXMoteur/Component.h"
#include "../DirectXMoteur/MeshRenderer.h"
#include "../DirectXMoteur/ShaderStructure.h"
#include "../DirectXMoteur/Script.h"

class GlobalScript : public Script
{
public:
	GlobalScript();
	~GlobalScript();

	void start(WindowEngine* wng);
	void update();

	entityManager* entityManage;

	Component* m_Component;
	Mesh mesh;
	MeshRenderer meshRender;
	Shader shader;

};

