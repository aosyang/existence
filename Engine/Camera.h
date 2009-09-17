//-----------------------------------------------------------------------------------
/// Camera.h 摄像机类，实现了本地移动、根据变换生成视矩阵的功能
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
/// 摄像机
/// 
/// 可以在场景中移动，提供观察视点的摄像机。提供一个平头视截体，可供渲染剔除使用
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

	// 裁剪平面
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
	float		m_Pitch;					///< 俯仰角度
	float		m_Heading;					///< 环视角度

	Frustum		m_Frustum;					///< 平头视截体
	float		m_FOVy;						///< Field of view Y
	float		m_Aspect;					///< 纵横比
	float		m_NearClippingDistance;		///< 近裁剪距离
	float		m_FarClippingDistance;		///< 远裁剪距离

	Matrix4		m_ViewMatrix;				///< 视矩阵
	Matrix4		m_ViewOffset;				///< 视矩阵偏移，影响视矩阵而不影响摄像机原始位置，用于产生镜头振动等效果
	//Matrix4		m_Transform;				///< 摄像机变换矩阵

	//Matrix3		m_YawMatrix;				///< 水平方向矩阵
	//Matrix3		m_PitchMatrix;				///< 俯仰方向矩阵

	//float		m_MoveSpeed;
	//float		m_RotateSpeed;

	bool		m_RestrictPitch;			///< 是否限制俯仰角
	float		m_PitchMax;					///< 俯仰角最大值(单位：角度)
	float		m_PitchMin;					///< 俯仰角最小值(单位：角度)

	bool		m_MatrixOutOfData;			///< 标记矩阵是否需要更新
};

#endif
