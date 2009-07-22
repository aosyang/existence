//-----------------------------------------------------------------------------------
/// Matrix3.h 3x3æÿ’Û
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#ifndef _MATRIX3_H
#define _MATRIX3_H


#include "Quaternion.h"
#include "EulerAngle.h"
#include "Vector3f.h"
#include <memory.h>

class Matrix3
{
public:
	union
	{
		float m[3][3];
		float mArray[9];
		float m11, m12, m13, m21, m22, m23, m31, m32, m33;
	};

	Matrix3() { memset(&this->m, 0, sizeof(float) * 9); }
	Matrix3(float _m11, float _m12, float _m13,
				   float _m21, float _m22, float _m23,
				   float _m31, float _m32, float _m33)
	{
		m[0][0] = _m11; m[0][1] = _m12; m[0][2] = _m13;
		m[1][0] = _m21; m[1][1] = _m22; m[1][2] = _m23;
		m[2][0] = _m31; m[2][1] = _m32; m[2][2] = _m33;
	}

	Matrix3& operator=(const Matrix3& rhs)
	{
		if (this != &rhs)
		{
			memcpy(this->m, &(rhs.m), sizeof(float) * 9);
		}

		return *this;
	}

	inline float* operator[] (size_t iRow) const { return (float*)m[iRow]; }

	Matrix3 operator* (const Matrix3& rkMatrix) const;
	Vector3f operator* (const Vector3f& rkPoint) const;

	Matrix3 operator*=(const Matrix3& matrix);

	void Identity();
	Matrix3 GetInverseMatrix() const;

	void setup(const EulerAngle &orinentation);

	void fromInertialToObjectQuaternion(const Quaternion &q);
	void fromObjectToInertialQuaternion(const Quaternion &q);

	Vector3f InertialToObject(const Vector3f &v) const;
	Vector3f ObjectToInertial(const Vector3f &v) const;

	static Matrix3 BuildYawRotationMatrix(float rad_angle);
	static Matrix3 BuildPitchRotationMatrix(float rad_angle);
	static Matrix3 BuildRollRotationMatrix(float rad_angle);
};

#endif
