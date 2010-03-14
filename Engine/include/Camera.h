//-----------------------------------------------------------------------------------
/// Camera.h ������࣬ʵ���˱����ƶ������ݱ任�����Ӿ���Ĺ���
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#ifndef _CAMERA_H
#define _CAMERA_H

#include "MathUtil.h"
#include "Vector3f.h"
#include "Matrix4.h"

#include "SceneObject.h"
#include "Frustum.h"
#include "Ray.h"

namespace Gen
{
	/// @brief
	/// �����
	/// @par
	///		�����ڳ������ƶ����ṩ�۲��ӵ����������ṩһ��ƽͷ�ӽ��壬�ɹ���Ⱦ�޳�ʹ��
	class Camera : public SceneObject
	{
		DECLARE_FACTORY_OBJECT(Camera, SceneObject);
	public:
		// ----- Overwrite IObject

		/// @copydoc SceneObject::Update()
		void Update(unsigned long deltaTime);

		// ----- Overwrite SceneObject

		/// @copydoc SceneObject::DebugRender()
		/// ��Ⱦ�������ƽͷ�ӽ���
		void DebugRender();

		// ----- Overwrite MovableObjectBase

		/// @copydoc MovableObjectBase::SetPosition()
		void SetPosition(const Vector3f& pos)
		{
			SceneObject::SetPosition(pos);
			m_MatrixOutOfData = true;
		}

		/// @copydoc MovableObjectBase::SetRotation()
		void SetRotation(const Quaternion& rot)
		{
			SceneObject::SetRotation(rot);
			m_MatrixOutOfData = true;
		}

		// ----- Camera Mathods

		/// @brief
		/// ʹ��������б���λ��
		/// @param forwardAmount
		///		ǰ����λ����
		/// @param rightAmount
		///		�ҷ���λ����
		/// @param upAmount
		///		�Ϸ���λ����
		/// @remarks
		///		�������λ������Ϊ0���򲻽��и��¡�
		///		����������ı��ر任����������ƽ�ƣ�������ʾ���Ӧ����ķ������ƶ���
		void MoveLocal(float forwardAmount, float rightAmount, float upAmount);

		/// @brief
		/// �����������ת
		/// @param headingAmount
		///		ˮƽ������ת��
		/// @param pitchAmount
		///		��ֱ������ת��
		/// @remarks
		///		�����������Ϊ0�򲻽��м��㡣
		///		FPSģʽ���������ֻ��Yaw��Pitch���п��ƣ����ݳ�Աm_RestrictPitch
		///		�����޶�Pitch��ת��Χ��
		void RotateLocal(float headingAmount, float pitchAmount);

		/// ��ȡƽͷ�ӽ���
		Frustum* GetFrustum() { return &m_Frustum; }

		/// @brief
		/// ͨ����Ļ�����ȡһ������ռ������
		/// @param x
		///		��Ļx����
		/// @param y
		///		��Ļy����
		/// @remarks
		///		x��y��ȡֵ��ΧΪ0.0 ~ 1.0����Ļ���Ͻ�Ϊ0, 0�����½�Ϊ1, 1��ˮƽ����Ϊx�ᣬ��ֱ����Ϊy��
		/// @par
		///		��������������������Ļ�ϵ�����ȡ����ռ������
		Ray GetCameraRay(float x, float y);

		/// ��ȡ�������ֱ�ӽ�
		float GetFOVy() const { return m_FOVy; }
		
		/// ָ���������ֱ�ӽ�
		void SetFOVy(float fovy) { m_FOVy = fovy; }

		/// ��ȡ������ݺ����
		float GetAspect() const { return m_Aspect; }

		/// ָ��������ݺ����
		/// @param aspect
		///		�ݺ��ֵ
		/// @remarks
		///		�������������������ƽͷ�ӽ���
		void SetAspect(float aspect);

		// �ü�ƽ��

		/// ��ȡ���ü�ƽ�����
		float GetNearClippingDistance() const { return m_NearClippingDistance; }
		/// ָ�����ü�ƽ�����
		void SetNearClippingDistance(float near_val)
		{
			m_NearClippingDistance = near_val;
			m_MatrixOutOfData = true;
		}

		/// ��ȡԶ�ü�ƽ�����
		float GetFarClippingDistance() const { return m_FarClippingDistance; }

		/// ָ��Զ�ü�ƽ�����
		void SetFarClippingDistance(float far_val)
		{
			m_FarClippingDistance = far_val;
			m_MatrixOutOfData = true;
		}

		/// ����������ӵ�ƫ�ƾ���
		/// @param viewOffset
		///		�ӵ�ƫ�ƾ���
		/// @remarks
		///		�ӵ�ƫ�ƾ��󽫻�Ӱ�쵽��������շ��ص��ӵ���󣬵��ǲ�Ӱ����������Ŀռ����꣬�������ڲ����������Ч��
		void SetViewOffsetMatrix(const Matrix4& viewOffset);

		/// ��ȡ�������ͶӰ����
		const Matrix4& GetProjMatrix();
		/// ��ȡ��������Ӿ���
		const Matrix4& GetViewMatrix();

		/// �����������ƽͷ�ӽ���
		void UpdateFrustum();

	private:
		/// ʹ�õ�ǰ�������λ�ø����Ӿ���
		void UpdateViewMatrix();

	protected:
		float		m_Pitch;					///< �����Ƕ�
		float		m_Heading;					///< ���ӽǶ�

		Frustum		m_Frustum;					///< ƽͷ�ӽ���
		float		m_FOVy;						///< ��ֱ������Ұ��Χ(��λ���Ƕ�)
		float		m_Aspect;					///< �ݺ��
		float		m_NearClippingDistance;		///< ���ü�����
		float		m_FarClippingDistance;		///< Զ�ü�����

		Matrix4		m_ViewMatrix;				///< �Ӿ���
		Matrix4		m_ViewOffset;				///< �Ӿ���ƫ�ƣ�Ӱ���Ӿ������Ӱ�������ԭʼλ�ã����ڲ�����ͷ�񶯵�Ч��
		//Matrix4		m_Transform;				///< ������任����

		//Matrix3		m_YawMatrix;				///< ˮƽ�������
		//Matrix3		m_PitchMatrix;				///< �����������

		//float		m_MoveSpeed;
		//float		m_RotateSpeed;

		bool		m_RestrictPitch;			///< �Ƿ����Ƹ�����
		float		m_PitchMax;					///< ���������ֵ(��λ���Ƕ�)
		float		m_PitchMin;					///< ��������Сֵ(��λ���Ƕ�)

		bool		m_MatrixOutOfData;			///< ��Ǿ����Ƿ���Ҫ����
	};
}

#endif
