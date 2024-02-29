#pragma once
class Component
{
public:
	Component() {};
	virtual void Component() = 0;
	~Component() {};
	virtual void ~Component() = 0;
};

