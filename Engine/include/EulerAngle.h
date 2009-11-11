//-----------------------------------------------------------------------------------
/// EulerAngle.h 欧拉角
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#ifndef __EULERANGLE_H_INCLUDED__
#define __EULERANGLE_H_INCLUDED__

namespace Gen
{
	class Quaternion;
	class Matrix4;
	class Matrix3;

	class EulerAngle
	{
	public:
		float heading;
		float pitch;
		float bank;

		EulerAngle(){}

		EulerAngle(float h, float p, float b):heading(h), pitch(p), bank(b){}

		void identity(){pitch = bank = heading = 0.0f;}

		void canonize();		//变为符合限制的欧拉角

		void fromObjectToInertialQuaternion(const Quaternion &q);		//从物体到惯性的四元数转换为欧拉角
		void fromInertialToObjectQuaternion(const Quaternion &q);		//从惯性到物体的四元数转换为欧拉角

		void fromObjectToWorldMatrix( const Matrix4 &m);				//从物体到世界的矩阵转换为欧拉角
		void fromWorldToObjectMatrix( const Matrix4 &m);				//从世界到物体的矩阵转换为欧拉角

		void fromRotationMatrix(const Matrix3 &m);
	};

	extern const EulerAngle kEulerAngleIdentity;
}

#endif


