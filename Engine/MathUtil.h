//-----------------------------------------------------------------------------------
/// MathUtil.h 数学工具
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#ifndef __MATHUTIL_H_INCLUDEED__
#define __MATHUTIL_H_INCLUDEED__

#include <math.h>
#include <stdlib.h>
#include <time.h>

// 屏蔽Win32的min和max宏定义
#define NOMINMAX

const float kPi = 3.14159265f;
const float k2Pi = kPi * 2.0f;
const float kPiOver2 = kPi/2.0f;
const float k1OverPi = 1.0f / kPi;
const float k1Over2Pi = 1.0f / k2Pi;

#define DEG_TO_RAD(deg) (deg) * kPi / 180

extern float wrapPi(float theta);	//限制角度在-Pi到Pi的区间内

extern float safeAcos(float x);		//安全反三角函数

inline void sinCos(float *returnSin, float *returnCos, float theta)
{
	*returnSin = sin(theta);
	*returnCos = cos(theta);
}

#define EPSILON 0.00001f
#define FLOAT_EQUALS_ZERO(num) (fabs(num) <= EPSILON)
#define FLOAT_EQUAL(a, b) (fabs(a-b) <= EPSILON)

#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

// Clamp，将数值限定在给定的最大值和最小值之间，
// 超出最大值则取最大值，不足最小值则取最小值
template <typename T> inline T MathClamp(const T& min, const T& max, const T& value)
{
	//if (value  >= max) return max;
	//else if (value <= min) return min;
	//else return value;

	return (value >= max) ? (max) : ((value <= min) ? (min) : (value));
}

// Repeat，将数值限定在给定的最大值和最小值之间
// 超出边界部分将从另一个边界折回
template <typename T> inline T MathRepeat(const T& min, const T& max, const T& value)
{
	T val = max - min;
	T result = value;
	while (result >= max)
		result -= val;

	while (result < min)
		result += val;

	return result;
}

void InitRandom();

// 给出一个指定闭区间上的随机数
template <typename T> inline T MathRandom(const T& min, const T& max)
{
	return min + (max - min) * (float)rand()/RAND_MAX;
}

#endif

