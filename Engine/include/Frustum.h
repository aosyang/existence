//-----------------------------------------------------------------------------------
/// Frustum.h ƽͷ�ӽ���
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

		// ����ͶӰ������Ӿ��������ӽ���ƽ��
		void BuildFrustumPlanes(const Matrix4& projMatrix, const Matrix4& viewMatrix);

		// ����-�ӽ��塱���
		bool IsPointInFrustum(const Vector3f& point);

		// ��AABB�ཻ�ж�
		bool IntersectsAABB(const AABB& aabb);

		// ������-�ӽ��塱���
		float SphereInFrustum(const Vector3f& point, float radius);

		Plane3 GetFrustumPlane(FrustumPlane plane) const { return m_FrustumPlanes[plane]; }
	private:
		Plane3			m_FrustumPlanes[6];			///< �ӽ���ƽ��
	};
}

#endif
