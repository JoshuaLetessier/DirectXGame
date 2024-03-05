#include "Timer.h"

Timer::Timer()
{
	// Initialize the start time to zero
	m_Start = std::chrono::high_resolution_clock::now();
}

Timer::~Timer()
{
}

void Timer::Start()
{
	// Set the start time to the current time
	m_Start = std::chrono::high_resolution_clock::now();
}

void Timer::Stop()
{
	// Set the stop time to the current time
	m_Stop = std::chrono::high_resolution_clock::now();
}

void Timer::Reset()
{
	// Reset the start time to the current time
	m_Start = std::chrono::high_resolution_clock::now();
}

float Timer::GetMilisecondsElapsed()
{
	// Get the current time and subtract the start time from it
	std::chrono::duration<float> elapsed = std::chrono::high_resolution_clock::now() - m_Start;

	// Return the elapsed time in milliseconds
	return elapsed.count() * 1000.0f;
}

float Timer::GetMilisecondsElapsedTotal()
{
	// Get the current time and subtract the start time from it
	std::chrono::duration<float> elapsed = std::chrono::high_resolution_clock::now() - m_Start;

	// Return the elapsed time in milliseconds
	return elapsed.count() * 1000.0f;
}

