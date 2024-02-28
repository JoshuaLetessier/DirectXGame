#pragma once
#include "pch.h"

class Trigger
{
private:
	bool m_triggered;

public:
	Trigger();
	~Trigger();

	void SetTriggered(bool triggered);
	bool GetTriggered() const;

	void Update();
};
