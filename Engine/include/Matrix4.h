//-----------------------------------------------------------------------------------
/// Matrix4.h 4x4矩阵
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#ifndef _MATRIX4_H
#define _MATRIX4_H

#include "Matrix3.h"
#include "Vector3f.h"
#include "Quaternion.h"


namespace Gen
{
	//-----------------------------------------------------------------------------------
	/// \brief
	/// 4x4变换矩阵
	/// 
	/// Write detailed description for Matrix4 here.
	/// 
	/// \remarks
	/// 4x4变换矩阵，成员排布如下<br>
	/// [m00, m01, m02, m03]<br>
	/// |m10, m11, m12, m13|<br>
	/// |m20, m21, m22, m23|<br>
	/// [m30, m31, m32, m33]<br>
	/// 
	//-----------------------------------------------------------------------------------
	class Matrix4
	{
	public:
		union{
			float m[4][4];
			float mArray[16];			///< Row-major array
		};

		Matrix4();
		inline Matrix4(
			float m00, float m01, float m02, float m03,
			float m10, float m11, float m12, float m13,
			float m20, float m21, float m22, float m23,
			float m30, float m31, float m32, float m33 )
		{
			m[0][0] = m00;	m[0][1] = m01;	m[0][2] = m02;	m[0][3] = m03;
			m[1][0] = m10;	m[1][1] = m11;	m[1][2] = m12;	m[1][3] = m13;
			m[2][0] = m20;	m[2][1] = m21;	m[2][2] = m22;	m[2][3] = m23;
			m[3][0] = m30;	m[3][1] = m31;	m[3][2] = m32;	m[3][3] = m33;
		}

		~Matrix4();

		Matrix4& operator=(const Matrix4& rhs)
		{
			if (this != &rhs)
			{
				memcpy(this->m, &(rhs.m), sizeof(float) * 16);
			}

			return *this;
		}
		void MakeIdentity();

		static Matrix4 IDENTITY;

		Matrix4 operator * (const Matrix4& m2) const;
		Vector3f operator * (const Vector3f& vec) const;

		Matrix4 operator*=(const Matrix4& m2);

		// 根据世界空间进行位移
		void TranslateWorld(const Vector3f& vec);
		// 根据自身空间方向进行位移
		void TranslateLocal(const Vector3f& vec);

		// 设置观察方向，仅影响旋转矩阵
		void SetLookAt(const Vector3f& direction, const Vector3f& up);

		// 获取右方向向量
		const Vector3f GetRightVector() const;
		// 获取上方向向量
		const Vector3f GetUpVector() const;
		// 获取前方向向量
		const Vector3f GetForwardVector() const;

		void SetRotation(const Quaternion& rot);

		void SetRotation(const Matrix3& rot);
		Matrix3 GetRotationMatrix() const;

		Matrix4 GetInverseMatrix() const;

		void SetPosition(const Vector3f& pos);
		inline const Vector3f GetPosition() const
		{
			return Vector3f(m[0][3], m[1][3], m[2][3]);
		}
	};
}

#endif


