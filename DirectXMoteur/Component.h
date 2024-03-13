#pragma once

class Entity;

class Component
{
public:
	Component() {}
	virtual ~Component() = default;

	void SetEntity(Entity* pEntity) {
		m_pEntity = pEntity; 
	}
	virtual bool Initialize() = 0;
	virtual void Update() = 0;

protected:
	Entity* m_pEntity = nullptr;
};