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
#include "Renderer.h"
#include "DebugRenderer.h"

namespace Gen
{
	// for debug render ray
	Vector3f nearPointWorld;
	Vector3f farPointWorld;

	Camera::Camera(SceneGraph* scene)
	: SceneObject(scene),
	  m_Pitch(0.0f), m_Heading(0.0f),
	  m_FOVy(45.0f), m_NearClippingDistance(1.0f), m_FarClippingDistance(100.0f),
	  m_RestrictPitch(true), m_PitchMax(90.0f), m_PitchMin(-90.0f),
	  m_MatrixOutOfData(true)
	{
		RenderWindowParam* param = System::Instance().GetRenderWindowParameters();
		m_Aspect = (float)param->width / param->height;

		m_ViewMatrix.MakeIdentity();
		m_ViewOffset.MakeIdentity();
		m_ProjMatrix.MakeIdentity();

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

		// note: 绑定在其他物体上的摄像机子节点无法仅使用变量判别是否需要更新，暂时全部更新

		// 如果视矩阵发生过改变，更新之
		//if (m_MatrixOutOfData)
		//{
			UpdateViewMatrix();
			UpdateFrustum();
			m_MatrixOutOfData = false;
		//}

	}
	
	void Camera::DebugRender()
	{
		// 使用最小和最大角坐标确定一个立方体
		Vector3f vMin = Vector3f(-1.0f, -1.0f, -1.0f);
		Vector3f vMax = Vector3f(1.0f, 1.0f, 1.0f);
	
		Matrix4 matViewProjInv = (m_ProjMatrix * m_ViewMatrix).GetInverseMatrix();
	
		// 渲染摄像机的视截体
		DebugRenderer::Instance().DrawBox(vMin, vMax, Color4f(1.0f, 1.0f, 1.0f), /*m_WorldTransform * */matViewProjInv);
		//DebugRenderer::Instance().DrawLine(nearPointWorld, farPointWorld);

		SceneObject::DebugRender();
	}

	void Camera::MoveLocal(float forwardAmount, float rightAmount, float upAmount)
	{
		if (forwardAmount == 0.0f && rightAmount == 0.0f && upAmount == 0.0f)
			return;

		m_Transform.TranslateLocal(Vector3f(rightAmount, upAmount, -forwardAmount));

		m_MatrixOutOfData = true;
	}

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

	void Camera::SetAspect(float aspect)
	{
		m_Aspect = aspect;
		UpdateFrustum();
	}

	// 根据屏幕上一点坐标，获取鼠标通过点击该点所产生的射线
	// 注：x, y的取值范围为  0.0 ~ 1.0
	Ray Camera::GetCameraRay(float x, float y)
	{
		Vector3f nearPoint = Vector3f(2.0f * x - 1.0f, - 2.0f * y + 1.0f, -1.0f);
		Vector3f farPoint = Vector3f(0.0f, 0.0f, 0.0f);
		//Vector3f farPoint = Vector3f(2.0f * x - 1.0f, - 2.0f * y + 1.0f, 0.0f);
		//Vector3f farLeftTop = Vector3f(1.0f, 1.0f, -1.0f);

		//Matrix4 matProjInv = m_Frustum.ProjectionMatrix().GetInverseMatrix();
		//Matrix4 matViewInv = m_ViewMatrix.GetInverseMatrix();
		Matrix4 matViewProjInv = (m_ProjMatrix * m_ViewMatrix).GetInverseMatrix();

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
		return m_ProjMatrix;
	}

	const Matrix4& Camera::GetViewMatrix()
	{
		return m_ViewMatrix;
	}

	void Camera::UpdateViewMatrix()
	{
		// View Matrix必须从世界空间“退回”摄像机移动所造成的修改
		m_ViewMatrix = (m_WorldTransform * m_ViewOffset).GetInverseMatrix();
	}

	void Camera::UpdateFrustum()
	{
		// 更新平头视截体的投影矩阵
		m_ProjMatrix = Matrix4::BuildPrespectiveProjMatrix(m_FOVy, m_Aspect, m_NearClippingDistance, m_FarClippingDistance);

		m_Frustum.BuildFrustumPlanes(m_ProjMatrix, m_ViewMatrix);
	}
}
