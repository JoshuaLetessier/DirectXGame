#pragma once
#include <windows.h>

class Timer
{
public:
	Timer();
	virtual ~Timer();

	void Start();
	bool Update();

	float GetElapsedTime();
	float GetTotalTime();
	void UpdateFPS();
	float GetFPS();

private:
	float mDeltaTime;
	float mTotalTime;
	DWORD mStart;
	DWORD mPrevious;
	DWORD mCountTime;
	DWORD mFPSPrevious;
	int mFPS;
	int mPrevFPS;
};
