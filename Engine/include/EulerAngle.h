//-----------------------------------------------------------------------------------
/// EulerAngle.h ŷ����
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

		void canonize();		//��Ϊ�������Ƶ�ŷ����

		void fromObjectToInertialQuaternion(const Quaternion &q);		//�����嵽���Ե���Ԫ��ת��Ϊŷ����
		void fromInertialToObjectQuaternion(const Quaternion &q);		//�ӹ��Ե��������Ԫ��ת��Ϊŷ����

		void fromObjectToWorldMatrix( const Matrix4 &m);				//�����嵽����ľ���ת��Ϊŷ����
		void fromWorldToObjectMatrix( const Matrix4 &m);				//�����絽����ľ���ת��Ϊŷ����

		void fromRotationMatrix(const Matrix3 &m);
	};

	extern const EulerAngle kEulerAngleIdentity;
}

#endif


