//-----------------------------------------------------------------------------------
/// Timer.h 计时器类，
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#ifndef _TIMER_H
#define _TIMER_H

// 这个类从Ogre复制而来

#include <windows.h>
#include <time.h>
#include <algorithm>

//Get around Windows hackery
#ifdef max
#  undef max
#endif
#ifdef min
#  undef min
#endif

class Timer
{
public:
	Timer();
	~Timer();

	void Reset();

	unsigned long GetMilliseconds();

protected:
	clock_t mZeroClock;

	DWORD mStartTick;
	LONGLONG mLastTime;
	LARGE_INTEGER mStartTime;
	LARGE_INTEGER mFrequency;

	DWORD mTimerMask;

};

#endif
