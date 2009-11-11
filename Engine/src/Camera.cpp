//-----------------------------------------------------------------------------------
/// Camera.cpp 摄像机类实现
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
		// 注：摄像机对象必须先更新自身矩阵，否则计算的视截体和视矩阵将是上一帧的位置
		SceneObject::Update(deltaTime);

		// 如果视矩阵发生过改变，更新之
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
	//	// 使用最小和最大角坐标确定一个立方体
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
	/// 摄像机本地位移
	/// 
	/// \param forwardAmount
	/// 前方向位移量
	/// 
	/// \param rightAmount
	/// 右方向位移量
	/// 
	/// \param upAmount
	/// 上方向位移量
	/// 
	/// 根据摄像机的本地变换向六个方向平移，负数表示向对应方向的反方向移动。
	/// 
	/// \remarks
	/// 如果三个位移量均为0，则不进行更新。
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
	/// 摄像机本地旋转
	/// 
	/// \param headingAmount
	/// 水平方向旋转量
	/// 
	/// \param pitchAmount
	/// 垂直方向旋转量
	/// 
	/// FPS模式的摄像机，只对Yaw和Pitch进行控制，根据成员m_RestrictPitch
	/// 可以限定Pitch旋转范围。
	/// 
	/// \remarks
	/// 如果两个量均为0则不进行计算。
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
	/// 设置摄像机的纵横比
	/// 
	/// \param aspect
	/// 纵横比值
	///
	/// 当窗体尺寸发生变化时，应当对每一个摄像机调用该方法重新设置纵横比
	/// 
	/// \remarks
	/// 这个方法将会重新计算平头视截体
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

		// 这方法使用近裁剪面和摄像机原点求出射线
		// FIXME: 靠近远裁剪面的地方射线范围呈弧形，无法与远裁剪面相交

		// TODO: 远裁剪面变换到世界空间似乎有问题，待研究

		nearPointWorld = m_WorldTransform.GetPosition();
		Vector3f rayDir = matViewProjInv * nearPoint - nearPointWorld;
		rayDir.normalize();
		farPointWorld = nearPointWorld + rayDir * m_FarClippingDistance;

		return Ray(nearPointWorld, farPointWorld);
	}

	// 指定视点偏移矩阵，用于产生摄像机振动效果
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
		// View Matrix必须从世界空间“退回”摄像机移动所造成的修改

		Matrix4 viewTransform = m_WorldTransform * m_ViewOffset;

		// 获取旋转矩阵的转置矩阵(3x3)
		m_ViewMatrix.m[0][0] = viewTransform.m[0][0]; m_ViewMatrix.m[1][0] = viewTransform.m[0][1]; m_ViewMatrix.m[2][0] = viewTransform.m[0][2]; 
		m_ViewMatrix.m[0][1] = viewTransform.m[1][0]; m_ViewMatrix.m[1][1] = viewTransform.m[1][1]; m_ViewMatrix.m[2][1] = viewTransform.m[1][2]; 
		m_ViewMatrix.m[0][2] = viewTransform.m[2][0]; m_ViewMatrix.m[1][2] = viewTransform.m[2][1]; m_ViewMatrix.m[2][2] = viewTransform.m[2][2]; 

		// 填充4x4视矩阵的其余位置
		m_ViewMatrix.m[3][0] = 0.0f;
		m_ViewMatrix.m[3][1] = 0.0f;
		m_ViewMatrix.m[3][2] = 0.0f;
		m_ViewMatrix.m[3][3] = 1.0f;

		// 计算位移
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
		// 更新平头视截体的投影矩阵
		m_Frustum.BuildPrespectiveProjMatrix(m_FOVy, m_Aspect, m_NearClippingDistance, m_FarClippingDistance);

		//m_Frustum.BuildOrthographicProjMatrix(-m_Aspect/2, m_Aspect/2, -0.5f, 0.5f, m_NearClippingDistance, m_FarClippingDistance);

		//renderer->ProjectionMatrix() = m_Frustum.ProjectionMatrix();

		m_Frustum.BuildFrustumPlanes(m_Frustum.ProjectionMatrix(), m_ViewMatrix/* * m_WorldTransform*/);

		//renderer->NotifyUpdateProjectionMatrix();

	}
}
