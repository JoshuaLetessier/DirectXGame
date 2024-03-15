#pragma once
#include "Component.h"
#include <vector>

using namespace std;

class Script : public Component
{
public:
	Script() {}
	~Script() {}

	bool Initialize() { return true; }
	void Update() {};
};

