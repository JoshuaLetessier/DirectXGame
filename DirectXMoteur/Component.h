#pragma once
#include <iostream>
#include <string>

class Entity;

class Component
{
public:
	Component() {};
	virtual ~Component() = default;

	void SetComponent(Entity* pEntity, const char* p_type) {
		m_pEntity = pEntity;
		m_type = p_type;
	}
	virtual bool Initialize() = 0;

	virtual void Update() = 0;

public:
	Entity* m_pEntity = nullptr;
	const char* m_type;
};