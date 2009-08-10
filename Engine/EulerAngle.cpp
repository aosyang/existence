//-----------------------------------------------------------------------------------
/// EulerAngle.cpp Å·À­½Ç
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#include <math.h>

#include "EulerAngle.h"
#include "Quaternion.h"
#include "MathUtil.h"
#include "Matrix4.h"
#include "Matrix3.h"

const EulerAngle kEulerAngleIdentity(0.0f, 0.0f, 0.0f);
using namespace Math;

void EulerAngle::canonize()
{
	pitch = WrapPi(pitch);

	if (pitch < -kPiOver2)		//make pitch between -Pi/2 and Pi/2
	{
		pitch = -kPi - pitch;
		heading += kPi;
		bank += kPi;
	}
	else if (pitch > kPiOver2)
	{
		pitch = kPi - pitch;
		heading += kPi;
		bank += kPi;
	}

	if (fabs(pitch) > kPiOver2 - 1e-4)
	{
		heading += bank;
		bank = 0.0f;
	}
	else 
	{
		bank = WrapPi(bank);
	}

	heading = WrapPi(heading);
}

void EulerAngle::fromObjectToInertialQuaternion(const Quaternion &q)
{
	float sp = -2.0f * (q.y * q.z - q.w * q.x);		//sp means sin(pitch)

	if (fabs(sp) > 0.9999f) 
	{

		// Looking straight up or down

		pitch = kPiOver2 * sp;

		// Compute heading, slam bank to zero

		heading = atan2(-q.x*q.z + q.w*q.y, 0.5f - q.y*q.y - q.z*q.z);
		bank = 0.0f;

	}
	else
	{

		// Compute angles.  We don't have to use the "safe" asin
		// function because we already checked for range errors when
		// checking for gimbal lock

		pitch	= asin(sp);
		heading	= atan2(q.x*q.z + q.w*q.y, 0.5f - q.x*q.x - q.y*q.y);
		bank	= atan2(q.x*q.y + q.w*q.z, 0.5f - q.x*q.x - q.z*q.z);
	}
}

void EulerAngle::fromInertialToObjectQuaternion(const Quaternion &q) {

	// Extract sin(pitch)

	float sp = -2.0f * (q.y*q.z + q.w*q.x);

	// Check for gimbal lock, giving slight tolerance for numerical imprecision

	if (fabs(sp) > 0.9999f)
	{

		// Looking straight up or down

		pitch = kPiOver2 * sp;

		// Compute heading, slam bank to zero

		heading = atan2(-q.x*q.z - q.w*q.y, 0.5f - q.y*q.y - q.z*q.z);
		bank = 0.0f;

	}
	else
	{

		// Compute angles.  We don't have to use the "safe" asin
		// function because we already checked for range errors when
		// checking for gimbal lock

		pitch	= asin(sp);
		heading	= atan2(q.x*q.z - q.w*q.y, 0.5f - q.x*q.x - q.y*q.y);
		bank	= atan2(q.x*q.y - q.w*q.z, 0.5f - q.x*q.x - q.z*q.z);
	}
}


void EulerAngle::fromObjectToWorldMatrix(const Matrix4 &m)
{
	float sp = -m.m[2][1];

	if (fabs(sp) > 9.9999f)
	{
		pitch = kPiOver2 * sp;

		heading = atan2(-m.m[1][2], m.m[0][0]);
		bank = 0.0f;
	}
	else
	{
		heading = atan2(-m.m[2][0], m.m[2][2]);
		pitch = asin(sp);
		bank = atan2(m.m[0][1], m.m[1][1]);
	}
}

void	EulerAngle::fromWorldToObjectMatrix(const Matrix4 &m) {

	// Extract sin(pitch) from m[1][2].

	float	sp = -m.m[1][2];

	// Check for gimbal lock

	if (fabs(sp) > 9.99999f) {

		// Looking straight up or down

		pitch = kPiOver2 * sp;

		// Compute heading, slam bank to zero

		heading = atan2(-m.m[2][0], m.m[0][0]);
		bank = 0.0f;

	}
	else
	{

		// Compute angles.  We don't have to use the "safe" asin
		// function because we already checked for range errors when
		// checking for gimbal lock

		heading = atan2(m.m[0][2], m.m[2][2]);
		pitch = asin(sp);
		bank = atan2(m.m[1][0], m.m[1][1]);
	}
}


void EulerAngle::fromRotationMatrix(const Matrix3 &m) {

	// Extract sin(pitch) from m[1][2].

	float	sp = -m.m[2][1];

	// Check for gimbal lock

	if (fabs(sp) > 9.99999f)
	{

		// Looking straight up or down

		pitch = kPiOver2 * sp;

		// Compute heading, slam bank to zero

		heading = atan2(-m.m[0][2], m.m[0][0]);
		bank = 0.0f;

	}
	else
	{

		// Compute angles.  We don't have to use the "safe" asin
		// function because we already checked for range errors when
		// checking for gimbal lock

		heading = atan2(m.m[2][0], m.m[2][2]);
		pitch = asin(sp);
		bank = atan2(m.m[0][1], m.m[1][1]);
	}
}


