//-----------------------------------------------------------------------------------
/// Matrix4.h 4x4����
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
	/// @brief
	/// 4x4�任����
	/// @remarks
	///		4x4�任���󣬳�Ա�Ų�����<br>
	///		[m00, m01, m02, m03]<br>
	///		|m10, m11, m12, m13|<br>
	///		|m20, m21, m22, m23|<br>
	///		[m30, m31, m32, m33]<br>
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

		/// @brief
		/// ����ֵ����
		/// @param rhs
		///		����һ������
		/// @returns
		///		���ص�ǰ����
		/// @remarks
		///		������ֵ������ʱ���������κβ���
		Matrix4& operator=(const Matrix4& rhs)
		{
			if (this != &rhs)
			{
				memcpy(this->m, &(rhs.m), sizeof(float) * 16);
			}

			return *this;
		}

		/// @brief
		///	��������Ϊ��λ�������Խ���ֵΪ1.0������Ϊ0.0
		void MakeIdentity();

		static Matrix4 IDENTITY;

		Matrix4 operator*(const Matrix4& m2) const;

		/// @brief
		///	�����������˷�����
		/// @param vec
		///		�������г˷����������
		/// @returns
		///		���س˷�����֮�������
		/// @remarks
		///		�����������������������г˷�����ʱΪ�ҳˡ���ά�����Զ���չΪ��ά�����������һ��ֵw��Ϊ1.0
		Vector3f operator*(const Vector3f& vec) const;

		Matrix4 operator*=(const Matrix4& m2);

		// ��������ռ����λ��
		void TranslateWorld(const Vector3f& vec);
		// ��������ռ䷽�����λ��
		void TranslateLocal(const Vector3f& vec);

		/// @brief
		/// ���þ���Ĺ۲췽��
		/// @param direction
		///		�۲췽������
		/// @param up
		///		�Ϸ�������
		/// @remarks
		///		����������ı䵱ǰ�������ת���󣬼����Ͻ�3x3�ķ�Χ
		void SetLookAt(const Vector3f& direction, const Vector3f& up);

		/// ��ȡ�ҷ�������
		const Vector3f GetRightVector() const;
		/// ��ȡ�Ϸ�������
		const Vector3f GetUpVector() const;
		/// ��ȡǰ��������
		const Vector3f GetForwardVector() const;

		void SetRotation(const Quaternion& rot);

		void SetRotation(const Matrix3& rot);
		Matrix3 GetRotationMatrix() const;

		/// @brief
		///	�������������
		/// @returns
		///		����õ��������
		Matrix4 GetInverseMatrix() const;

		void SetPosition(const Vector3f& pos);
		inline const Vector3f GetPosition() const
		{
			return Vector3f(m[0][3], m[1][3], m[2][3]);
		}

		static Matrix4 BuildPrespectiveProjMatrix(float fovy, float aspect, float _near, float _far);

		// ����͸��ͶӰ����
		static Matrix4 BuildPerspectiveProjection(float left, float right, float bottom, float top, float znear, float zfar);

		// ��������ͶӰ����
		static Matrix4 BuildOrthographicProjection(float left, float right, float bottom, float top, float znear, float zfar);
	};
}

#endif


