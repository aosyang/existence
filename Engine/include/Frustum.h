//-----------------------------------------------------------------------------------
/// Frustum.h 平头视截体
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------

#ifndef _FRUSTUM_H
#define _FRUSTUM_H

#include "Matrix4.h"
#include "Vector3f.h"
#include "Plane3.h"
#include "AABB.h"

namespace Gen
{
	enum FrustumPlane
	{
		FRUSTUM_PLANE_RIGHT,
		FRUSTUM_PLANE_LEFT,
		FRUSTUM_PLANE_BOTTOM,
		FRUSTUM_PLANE_TOP,
		FRUSTUM_PLANE_FAR,
		FRUSTUM_PLANE_NEAR,
	};

	class Frustum
	{
	public:
		Frustum();

		// 生成透视投影矩阵
		Matrix4 BuildPrespectiveProjMatrix(float fovy = 45.0f, float aspect = 4.0f/3.0f, float near = 1.0f, float far = 100.0f);

		// 根据投影矩阵和视矩阵生成视截体平面
		void BuildFrustumPlanes(const Matrix4& projMatrix, const Matrix4& viewMatrix);

		// “点-视截体”检测
		bool IsPointInFrustum(const Vector3f& point);

		// 与AABB相交判断
		bool IntersectsAABB(const AABB& aabb);

		// “球体-视截体”检测
		float SphereInFrustum(const Vector3f& point, float radius);

		Matrix4& ProjectionMatrix() { return m_ProjMatrix; }

		Plane3 GetFrustumPlane(FrustumPlane plane) const { return m_FrustumPlanes[plane]; }

		float	m_Left, m_Right, m_Bottom, m_Top;
	private:
		Plane3			m_FrustumPlanes[6];			///< 视截体平面

		Matrix4			m_ProjMatrix;
	};
}

#endif
