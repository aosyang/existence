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

//-----------------------------------------------------------------------------------
/// \brief
/// �����
/// 
/// �����ڳ������ƶ����ṩ�۲��ӵ����������ṩһ��ƽͷ�ӽ��壬�ɹ���Ⱦ�޳�ʹ��
/// 
/// \remarks
/// Write remarks for Camera here.
/// 
/// \see
/// Separate items with the '|' character.
//-----------------------------------------------------------------------------------
class Camera : public SceneObject
{
public:
	Camera();

	// ----- Overwrite IObject

	void Update(unsigned long deltaTime);
	//void DebugRender();

	const String GetTypeName() const { return "Camera"; }

	// ----- Overwrite BaseSceneObject

	void SetPosition(const Vector3f& pos)
	{
		SceneObject::SetPosition(pos);
		m_MatrixOutOfData = true;
	}

	void SetRotation(const Quaternion& rot)
	{
		SceneObject::SetRotation(rot);
		m_MatrixOutOfData = true;
	}

	// ----- Camera Mathods

	void MoveLocal(float forwardAmount, float rightAmount, float upAmount);

	void RotateLocal(float headingAmount, float pitchAmount);

	Frustum* GetFrustum() { return &m_Frustum; }
	Ray GetCameratRay(float x, float y);

	float GetFOVy() const { return m_FOVy; }
	void SetFOVy(float fovy) { m_FOVy = fovy; }

	float GetAspect() const { return m_Aspect; }
	void SetAspect(float aspect);

	// �ü�ƽ��
	float GetNearClippingDistance() const { return m_NearClippingDistance; }
	void SetNearClippingDistance(float near_val)
	{
		m_NearClippingDistance = near_val;
		m_MatrixOutOfData = true;
	}

	float GetFarClippingDistance() const { return m_FarClippingDistance; }
	void SetFarClippingDistance(float far_val)
	{
		m_FarClippingDistance = far_val;
		m_MatrixOutOfData = true;
	}

	void SetViewOffsetMatrix(const Matrix4& viewOffset);

	const Matrix4& GetProjMatrix();
	const Matrix4& GetViewMatrix();

	void UpdateFrustum();

private:
	void UpdateViewMatrix();

protected:
	float		m_Pitch;					///< �����Ƕ�
	float		m_Heading;					///< ���ӽǶ�

	Frustum		m_Frustum;					///< ƽͷ�ӽ���
	float		m_FOVy;						///< Field of view Y
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

#endif
