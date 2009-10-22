//-----------------------------------------------------------------------------------
/// MathUtil.cpp 数学工具
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#include "MathUtil.h"

#include "Vector3f.h"

namespace Math
{
	const Vector3f kZeroVector(0.0f, 0.0f, 0.0f);

	float WrapPi(float theta)	//通过加上适当的2Pi的倍数，将角度限制在-Pi到Pi的区间内
	{
		theta += kPi;
		theta -= floor(theta * k1Over2Pi) * k2Pi;
		theta -= kPi;
		return theta;
	}

	float safeAcos(float x)
	{
		if ( x <= - 1.0f)return kPi;
		if ( x >= 1.0f) return 0.0f;

		return acos(x);
	}

	void InitRandom()
	{
		srand((unsigned int)(time(NULL)));
	}

}
