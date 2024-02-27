#pragma once
#include "pch.h"

class Trigger
{
private:
	bool mtriggered;

public:
	Trigger();
	~Trigger();

	void SetTriggered(bool mtriggered);
	bool GetTriggered() const;

	void Update();
};

