//#include "Timer.h"
//
//Timer::Timer() {}
//
//Timer::~Timer() {}
//
//void Timer::Start()
//{
//	mStart = timeGetTime();
//	mPrevious = 0;// mStart;
//	mTotalTime = 0.0f;
//	mDeltaTime = 0.0f;
//	mCountTime = 0;
//	mFPS = 0;
//	mPrevFPS = 0;
//}
//
//bool Timer::Update()
//{
//	//Current time - Previous Time
//	DWORD totalTimeMs = timeGetTime() - mStart;
//	DWORD dt = totalTimeMs - mPrevious;
//	if (dt < 10)
//	{
//		return false;
//	}
//	mPrevious = totalTimeMs;
//
//	UpdateFPS();
//
//
//	if (dt > 40)
//	{
//		dt = 40;
//	}
//
//	mDeltaTime = dt / 1000.0f;
//	mTotalTime += mDeltaTime;
//
//	return true;
//}
//
//float Timer::GetElapsedTime()
//{
//	return mDeltaTime;
//}
//
//float Timer::GetTotalTime()
//{
//	return mTotalTime;
//}
//
//void Timer::UpdateFPS()
//{
//	DWORD currentTime = timeGetTime();
//	DWORD elapsedTime = currentTime - mFPSPrevious;
//	mFPSPrevious = currentTime;
//
//	mCountTime += elapsedTime;
//	mFPS++;
//
//	if (mCountTime >= 1000)
//	{
//		mPrevFPS = mFPS;
//		mFPS = 0;
//		mCountTime = 0;
//	}
//}
//
//
//float Timer::GetFPS()
//{
//	return mPrevFPS;
//}