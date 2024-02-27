#include "Trigger.h"

Trigger::Trigger()
{
	mtriggered = false;
}

Trigger::~Trigger()
{
}

void Trigger::SetTriggered()
{
	mtriggered = true;
}

bool Trigger::GetTriggered() const
{
	return mtriggered;
}

void Trigger::Update()
{
	if (mtriggered)
	{
		// Do something
	}
}