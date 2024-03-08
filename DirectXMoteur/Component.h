#pragma once

class Component
{
public:
	Component() {}
	virtual ~Component() = default;

	virtual bool Initialize() = 0;
	virtual void Update() = 0;
}; 