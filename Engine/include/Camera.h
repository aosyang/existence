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

namespace Gen
{
	/// @brief
	/// 摄像机
	/// @par
	///		可以在场景中移动，提供观察视点的摄像机。提供一个平头视截体，可供渲染剔除使用
	class Camera : public SceneObject
	{
		DECLARE_FACTORY_OBJECT(Camera, SceneObject);
	public:
		// ----- Overwrite IObject

		/// @copydoc SceneObject::Update()
		void Update(unsigned long deltaTime);

		// ----- Overwrite SceneObject

		/// @copydoc SceneObject::DebugRender()
		/// 渲染摄像机的平头视截体
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
		/// 使摄像机进行本地位移
		/// @param forwardAmount
		///		前方向位移量
		/// @param rightAmount
		///		右方向位移量
		/// @param upAmount
		///		上方向位移量
		/// @remarks
		///		如果三个位移量均为0，则不进行更新。
		///		根据摄像机的本地变换向六个方向平移，负数表示向对应方向的反方向移动。
		void MoveLocal(float forwardAmount, float rightAmount, float upAmount);

		/// @brief
		/// 摄像机本地旋转
		/// @param headingAmount
		///		水平方向旋转量
		/// @param pitchAmount
		///		垂直方向旋转量
		/// @remarks
		///		如果两个量均为0则不进行计算。
		///		FPS模式的摄像机，只对Yaw和Pitch进行控制，根据成员m_RestrictPitch
		///		可以限定Pitch旋转范围。
		void RotateLocal(float headingAmount, float pitchAmount);

		/// 获取平头视截体
		Frustum* GetFrustum() { return &m_Frustum; }

		/// @brief
		/// 通过屏幕坐标获取一个世界空间的射线
		/// @param x
		///		屏幕x坐标
		/// @param y
		///		屏幕y坐标
		/// @remarks
		///		x与y的取值范围为0.0 ~ 1.0，屏幕左上角为0, 0，右下角为1, 1，水平方向为x轴，垂直方向为y轴
		/// @par
		///		这个方法多用于鼠标在屏幕上点击后获取世界空间的射线
		Ray GetCameraRay(float x, float y);

		/// 获取摄像机垂直视角
		float GetFOVy() const { return m_FOVy; }
		
		/// 指定摄像机垂直视角
		void SetFOVy(float fovy) { m_FOVy = fovy; }

		/// 获取摄像机纵横比例
		float GetAspect() const { return m_Aspect; }

		/// 指定摄像机纵横比例
		/// @param aspect
		///		纵横比值
		/// @remarks
		///		这个方法会更新摄像机的平头视截体
		void SetAspect(float aspect);

		// 裁剪平面

		/// 获取近裁剪平面距离
		float GetNearClippingDistance() const { return m_NearClippingDistance; }
		/// 指定近裁剪平面距离
		void SetNearClippingDistance(float near_val)
		{
			m_NearClippingDistance = near_val;
			m_MatrixOutOfData = true;
		}

		/// 获取远裁剪平面距离
		float GetFarClippingDistance() const { return m_FarClippingDistance; }

		/// 指定远裁剪平面距离
		void SetFarClippingDistance(float far_val)
		{
			m_FarClippingDistance = far_val;
			m_MatrixOutOfData = true;
		}

		/// 设置摄像机视点偏移矩阵
		/// @param viewOffset
		///		视点偏移矩阵
		/// @remarks
		///		视点偏移矩阵将会影响到摄像机最终返回的视点矩阵，但是不影响摄像机结点的空间坐标，可以用于产生摄像机震动效果
		void SetViewOffsetMatrix(const Matrix4& viewOffset);

		/// 获取摄像机的投影矩阵
		const Matrix4& GetProjMatrix();
		/// 获取摄像机的视矩阵
		const Matrix4& GetViewMatrix();

		/// 更新摄像机的平头视截体
		void UpdateFrustum();

	private:
		/// 使用当前摄像机的位置更新视矩阵
		void UpdateViewMatrix();

	protected:
		float		m_Pitch;					///< 俯仰角度
		float		m_Heading;					///< 环视角度

		Frustum		m_Frustum;					///< 平头视截体
		float		m_FOVy;						///< 垂直方向视野范围(单位：角度)
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
}

#endif
