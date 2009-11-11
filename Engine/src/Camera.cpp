//-----------------------------------------------------------------------------------
/// Camera.cpp �������ʵ��
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#include "Camera.h"
#include "Engine.h"
#include "System.h"

namespace Gen
{
	// for debug render ray
	Vector3f nearPointWorld;
	Vector3f farPointWorld;

	Camera::Camera()
	: m_Pitch(0.0f), m_Heading(0.0f),
	  m_FOVy(45.0f), m_NearClippingDistance(1.0f), m_FarClippingDistance(100.0f),
	  m_RestrictPitch(true), m_PitchMax(90.0f), m_PitchMin(-90.0f),
	  m_MatrixOutOfData(true)
	{
		RenderWindowParam* param = System::Instance().GetRenderWindowParameters();
		m_Aspect = (float)param->width / param->height;

		m_ViewMatrix.MakeIdentity();
		m_ViewOffset.MakeIdentity();

		//m_YawMatrix.MakeIdentity();
		//m_PitchMatrix.MakeIdentity();

		UpdateFrustum();
	}

	Camera::~Camera()
	{
	}

	void Camera::Update(unsigned long deltaTime)
	{
		// ע���������������ȸ���������󣬷��������ӽ�����Ӿ�������һ֡��λ��
		SceneObject::Update(deltaTime);

		// ����Ӿ��������ı䣬����֮
		if (m_MatrixOutOfData)
		{
			UpdateViewMatrix();
			UpdateFrustum();
			m_MatrixOutOfData = false;
		}

	}
	//
	//void Camera::DebugRender()
	//{
	//	BaseSceneObject::DebugRender();
	//
	//	// ʹ����С����������ȷ��һ��������
	//	Vector3f vMin = Vector3f(-1.0f, -1.0f, -1.0f);
	//	Vector3f vMax = Vector3f(1.0f, 1.0f, 1.0f);
	//
	//	Matrix4 matViewProjInv = (m_Frustum.ProjectionMatrix() * m_ViewMatrix).GetInverseMatrix();
	//
	//	renderer->RenderBox(vMin, vMax, Color4f(1.0f, 1.0f, 1.0f), /*m_WorldTransform * */matViewProjInv);
	//	renderer->RenderLine(nearPointWorld, farPointWorld);
	//}

	//-----------------------------------------------------------------------------------
	/// \brief
	/// ���������λ��
	/// 
	/// \param forwardAmount
	/// ǰ����λ����
	/// 
	/// \param rightAmount
	/// �ҷ���λ����
	/// 
	/// \param upAmount
	/// �Ϸ���λ����
	/// 
	/// ����������ı��ر任����������ƽ�ƣ�������ʾ���Ӧ����ķ������ƶ���
	/// 
	/// \remarks
	/// �������λ������Ϊ0���򲻽��и��¡�
	//-----------------------------------------------------------------------------------
	void Camera::MoveLocal(float forwardAmount, float rightAmount, float upAmount)
	{
		if (forwardAmount == 0.0f && rightAmount == 0.0f && upAmount == 0.0f)
			return;

		m_Transform.TranslateLocal(Vector3f(rightAmount, upAmount, -forwardAmount));

		m_MatrixOutOfData = true;
	}

	//-----------------------------------------------------------------------------------
	/// \brief
	/// �����������ת
	/// 
	/// \param headingAmount
	/// ˮƽ������ת��
	/// 
	/// \param pitchAmount
	/// ��ֱ������ת��
	/// 
	/// FPSģʽ���������ֻ��Yaw��Pitch���п��ƣ����ݳ�Աm_RestrictPitch
	/// �����޶�Pitch��ת��Χ��
	/// 
	/// \remarks
	/// �����������Ϊ0�򲻽��м��㡣
	//-----------------------------------------------------------------------------------
	void Camera::RotateLocal(float headingAmount, float pitchAmount)
	{
		if (headingAmount == 0.0f && pitchAmount == 0.0f)
			return;

		Quaternion rot_heading, rot_pitch;

		if (headingAmount != 0.0f)
		{
			m_Heading += headingAmount;

			//m_YawMatrix = Matrix3::BuildYawRotationMatrix(DEG_TO_RAD(m_Heading));
		}

		if (pitchAmount != 0.0f)
		{
			m_Pitch += pitchAmount;

			if (m_RestrictPitch)
				m_Pitch = Math::Clamp(m_PitchMin, m_PitchMax, m_Pitch);

			//m_PitchMatrix = Matrix3::BuildPitchRotationMatrix(DEG_TO_RAD(m_Pitch));
		}
		rot_heading.CreateFromLocalAxisAngle(Vector3f(0.0f, 1.0f, 0.0f), DEG_TO_RAD(m_Heading));
		rot_pitch.CreateFromLocalAxisAngle(Vector3f(1.0f, 0.0f, 0.0f), DEG_TO_RAD(m_Pitch));

		//m_Transform.SetRotation(m_YawMatrix * m_PitchMatrix);
		SetRotation(rot_heading * rot_pitch);
		m_MatrixOutOfData = true;

	}

	//-----------------------------------------------------------------------------------
	/// \brief
	/// ������������ݺ��
	/// 
	/// \param aspect
	/// �ݺ��ֵ
	///
	/// ������ߴ緢���仯ʱ��Ӧ����ÿһ����������ø÷������������ݺ��
	/// 
	/// \remarks
	/// ��������������¼���ƽͷ�ӽ���
	//-----------------------------------------------------------------------------------
	void Camera::SetAspect(float aspect)
	{
		m_Aspect = aspect;
		UpdateFrustum();
	}

	// x, y  0.0 ~ 1.0
	Ray Camera::GetCameraRay(float x, float y)
	{
		Vector3f nearPoint = Vector3f(2.0f * x - 1.0f, - 2.0f * y + 1.0f, -1.0f);
		Vector3f farPoint = Vector3f(0.0f, 0.0f, 0.0f);
		//Vector3f farPoint = Vector3f(2.0f * x - 1.0f, - 2.0f * y + 1.0f, 0.0f);
		//Vector3f farLeftTop = Vector3f(1.0f, 1.0f, -1.0f);

		//Matrix4 matProjInv = m_Frustum.ProjectionMatrix().GetInverseMatrix();
		//Matrix4 matViewInv = m_ViewMatrix.GetInverseMatrix();
		Matrix4 matViewProjInv = (m_Frustum.ProjectionMatrix() * m_ViewMatrix).GetInverseMatrix();

		//nearPointWorld = matViewInv * nearPoint;
		//farPointWorld = matViewInv * farPoint;

		//nearPointWorld = nearPoint;
		//farPointWorld = farPoint;

		// �ⷽ��ʹ�ý��ü���������ԭ���������
		// FIXME: ����Զ�ü���ĵط����߷�Χ�ʻ��Σ��޷���Զ�ü����ཻ

		// TODO: Զ�ü���任������ռ��ƺ������⣬���о�

		nearPointWorld = m_WorldTransform.GetPosition();
		Vector3f rayDir = matViewProjInv * nearPoint - nearPointWorld;
		rayDir.normalize();
		farPointWorld = nearPointWorld + rayDir * m_FarClippingDistance;

		return Ray(nearPointWorld, farPointWorld);
	}

	// ָ���ӵ�ƫ�ƾ������ڲ����������Ч��
	void Camera::SetViewOffsetMatrix(const Matrix4& viewOffset)
	{
		m_ViewOffset = viewOffset;
		m_MatrixOutOfData = true;
	}

	const Matrix4& Camera::GetProjMatrix()
	{
		return m_Frustum.ProjectionMatrix();
	}

	const Matrix4& Camera::GetViewMatrix()
	{
		return m_ViewMatrix;
	}

	void Camera::UpdateViewMatrix()
	{
		// View Matrix���������ռ䡰�˻ء�������ƶ�����ɵ��޸�

		Matrix4 viewTransform = m_WorldTransform * m_ViewOffset;

		// ��ȡ��ת�����ת�þ���(3x3)
		m_ViewMatrix.m[0][0] = viewTransform.m[0][0]; m_ViewMatrix.m[1][0] = viewTransform.m[0][1]; m_ViewMatrix.m[2][0] = viewTransform.m[0][2]; 
		m_ViewMatrix.m[0][1] = viewTransform.m[1][0]; m_ViewMatrix.m[1][1] = viewTransform.m[1][1]; m_ViewMatrix.m[2][1] = viewTransform.m[1][2]; 
		m_ViewMatrix.m[0][2] = viewTransform.m[2][0]; m_ViewMatrix.m[1][2] = viewTransform.m[2][1]; m_ViewMatrix.m[2][2] = viewTransform.m[2][2]; 

		// ���4x4�Ӿ��������λ��
		m_ViewMatrix.m[3][0] = 0.0f;
		m_ViewMatrix.m[3][1] = 0.0f;
		m_ViewMatrix.m[3][2] = 0.0f;
		m_ViewMatrix.m[3][3] = 1.0f;

		// ����λ��
		float x = viewTransform.m[0][3];
		float y = viewTransform.m[1][3];
		float z = viewTransform.m[2][3];

		float vx = viewTransform.m[0][0] * x + viewTransform.m[1][0] * y + viewTransform.m[2][0] * z;
		float vy = viewTransform.m[0][1] * x + viewTransform.m[1][1] * y + viewTransform.m[2][1] * z;
		float vz = viewTransform.m[0][2] * x + viewTransform.m[1][2] * y + viewTransform.m[2][2] * z;

		m_ViewMatrix.m[0][3] = -vx;
		m_ViewMatrix.m[1][3] = -vy;
		m_ViewMatrix.m[2][3] = -vz;
	}

	void Camera::UpdateFrustum()
	{
		// ����ƽͷ�ӽ����ͶӰ����
		m_Frustum.BuildPrespectiveProjMatrix(m_FOVy, m_Aspect, m_NearClippingDistance, m_FarClippingDistance);

		//m_Frustum.BuildOrthographicProjMatrix(-m_Aspect/2, m_Aspect/2, -0.5f, 0.5f, m_NearClippingDistance, m_FarClippingDistance);

		//renderer->ProjectionMatrix() = m_Frustum.ProjectionMatrix();

		m_Frustum.BuildFrustumPlanes(m_Frustum.ProjectionMatrix(), m_ViewMatrix/* * m_WorldTransform*/);

		//renderer->NotifyUpdateProjectionMatrix();

	}
}
