#pragma once

#include <chrono>
#include "pch.h"

class Timer
{
public:
	Timer();
	~Timer();

	void Start();
	void Stop();
	void Reset();
	float GetMilisecondsElapsed();
	float GetMilisecondsElapsedTotal();

private:
	chrono::time_point<chrono::high_resolution_clock> m_Start;
	chrono::time_point<chrono::high_resolution_clock> m_Stop;
};
