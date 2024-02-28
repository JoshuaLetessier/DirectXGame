#pragma once
#include "pch.h"

class Collider
{
private:
	float x, y, width, height;
	bool isTrigger;
    std::vector<Collider*> colliders;
	std::vector<Collider*> collidingWith;
	int m_x, m_y, m_width, m_height;

public:

	Collider();
	~Collider();

	void SetCollider(float x, float y, float width, float height);
	void SetTrigger(bool isTrigger);
	void AddCollider(Collider* collider);
	void RemoveCollider(Collider* collider);
	void CheckCollision(Collider* collider);
	void CheckCollisions();
	void Draw();
	void Update();
	void OnCollision(Collider* collider);

};

