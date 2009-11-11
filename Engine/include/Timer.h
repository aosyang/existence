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

#include "Platform.h"

#if defined __PLATFORM_WIN32
#include <time.h>
#include <algorithm>

//Get around Windows hackery
#ifdef max
#  undef max
#endif
#ifdef min
#  undef min
#endif

#elif defined __PLATFORM_LINUX
#include <sys/time.h>
#include <time.h>

#endif	// #if defined __PLATFORM_WIN32

namespace Gen
{
	class Timer
	{
	public:
		Timer();
		~Timer();

		void Reset();

		unsigned long GetMilliseconds();

	protected:
#if defined __PLATFORM_WIN32
		clock_t mZeroClock;

		DWORD mStartTick;
		LONGLONG mLastTime;
		LARGE_INTEGER mStartTime;
		LARGE_INTEGER mFrequency;

		DWORD mTimerMask;

#elif defined __PLATFORM_LINUX
		struct timeval start;
		clock_t zeroClock;
#endif	// #if defined __PLATFORM_WIN32

	};
}

#endif
