//-----------------------------------------------------------------------------------
/// Quaternion.h 四元数
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
//#ifndef __QUATERNION_H_INCLUDED__
//#define __QUATERNION_H_INCLUDED__
//
//class Vector3f;
//class EulerAngle;
//class Matrix3;
//
//class Quaternion
//{
//public:
//	float w, x, y, z;
//
//	void identity(){ w = 1.0f; x = y = z = 0.0f;}
//
//	void setToRotateAboutX(float theta);
//	void setToRotateAboutY(float theta);
//	void setToRotateAboutZ(float theta);
//
//	void setToRotateAboutAxis(const Vector3f &axis, float theta);
//
//	void setToRotateObjectToInertial(const EulerAngle &orientation);
//	void setToRotateInertialToObject(const EulerAngle &orientation);
//
//	Matrix3 GetRotMatrix();
//
//	Quaternion operator *(const Quaternion &a) const;
//
//	Quaternion &operator *=(const Quaternion &a);
//
//	void normalize();
//
//	float getRotationAngle() const;
//	Vector3f getRotationAxis() const;
//};
//
//extern const Quaternion kQuaternionIdentity;
//
//extern float dotProduct(const Quaternion &a, const Quaternion &b);
//
//extern Quaternion slerp(const Quaternion &p, const Quaternion &q, float t);
//
//extern Quaternion conjugate (const Quaternion &q);			//共轭
//
//extern Quaternion pow(const Quaternion &q, float exponent);
//
//#endif

#ifndef _QUATERNION_H
#define _QUATERNION_H

#include "Matrix3.h"
#include "Vector3f.h"

namespace Gen
{
	class Matrix3;
	class Vector3f;

	class Quaternion
	{
	public:
		float x, y, z, w;

		Quaternion();
		Quaternion(const Matrix3& mat);

		Quaternion operator*(const Quaternion& rhs) const;
		Quaternion operator*=(const Quaternion& rhs);

		void Normalize();

		// 给定任意轴，在局部空间中沿该轴作逆时针旋转指定弧度
		void CreateFromLocalAxisAngle(const Vector3f axis, float angle_rad);

		Matrix3 GetRotationMatrix() const;

		static Quaternion Slerp(const Quaternion &p, const Quaternion &q, float t);
		static float DotProduct(const Quaternion &a, const Quaternion &b);
	};
}

#endif
