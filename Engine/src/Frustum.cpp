//-----------------------------------------------------------------------------------
/// Frustum.cpp ƽͷ�ӽ���
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#include "Frustum.h"

#include "MathUtil.h"

namespace Gen
{
	// TODO: ResizeWindow������Frustum��aspect ratio

	Frustum::Frustum()
		//: m_Near(1.0f), m_Far(100.0f),
		//  m_FOVy(45.0f), m_Aspect(4.0f/3.0f)
	{
	}

	Matrix4 Frustum::BuildPrespectiveProjMatrix(float fovy, float aspect, float _near, float _far)
	{
		float range = _near * tan(DEG_TO_RAD(fovy / 2));

		// ������Щֵ����������ӽ���ķ�Χ
		//m_Left = -range * aspect;
		//m_Right = range * aspect;
		//m_Bottom = -range;
		//m_Top = range;

		m_ProjMatrix = Matrix4::BuildPerspectiveProjection(-range * aspect, range * aspect, -range, range, _near, _far);
		return m_ProjMatrix;
	}

	// ��Щ��������︴�ƹ�����http://www.racer.nl/reference/vfc_markmorley.htm

	void Frustum::BuildFrustumPlanes(const Matrix4& projMatrix, const Matrix4& viewMatrix)
	{
		Matrix4 clipMatrix;
		float t;

		clipMatrix = projMatrix * viewMatrix;

		// ע���������������ʷ�ʽ��GL��ʽ��һ������Ҫ����һ��

		/* Extract the numbers for the RIGHT plane */
		m_FrustumPlanes[0][0] = clipMatrix.mArray[12] - clipMatrix.mArray[ 0];
		m_FrustumPlanes[0][1] = clipMatrix.mArray[13] - clipMatrix.mArray[ 1];
		m_FrustumPlanes[0][2] = clipMatrix.mArray[14] - clipMatrix.mArray[ 2];
		m_FrustumPlanes[0][3] = clipMatrix.mArray[15] - clipMatrix.mArray[ 3];
		/* Normalize the result */
		t = sqrt( m_FrustumPlanes[0][0] * m_FrustumPlanes[0][0] + m_FrustumPlanes[0][1] * m_FrustumPlanes[0][1] + m_FrustumPlanes[0][2]    * m_FrustumPlanes[0][2] );
		m_FrustumPlanes[0][0] /= t;
		m_FrustumPlanes[0][1] /= t;
		m_FrustumPlanes[0][2] /= t;
		m_FrustumPlanes[0][3] /= t;
		/* Extract the numbers for the LEFT plane */
		m_FrustumPlanes[1][0] = clipMatrix.mArray[12] + clipMatrix.mArray[ 0];
		m_FrustumPlanes[1][1] = clipMatrix.mArray[13] + clipMatrix.mArray[ 1];
		m_FrustumPlanes[1][2] = clipMatrix.mArray[14] + clipMatrix.mArray[ 2];
		m_FrustumPlanes[1][3] = clipMatrix.mArray[15] + clipMatrix.mArray[ 3];
		/* Normalize the result */
		t = sqrt( m_FrustumPlanes[1][0] * m_FrustumPlanes[1][0] + m_FrustumPlanes[1][1] * m_FrustumPlanes[1][1] + m_FrustumPlanes[1][2]    * m_FrustumPlanes[1][2] );
		m_FrustumPlanes[1][0] /= t;
		m_FrustumPlanes[1][1] /= t;
		m_FrustumPlanes[1][2] /= t;
		m_FrustumPlanes[1][3] /= t;
		/* Extract the BOTTOM plane */
		m_FrustumPlanes[2][0] = clipMatrix.mArray[12] + clipMatrix.mArray[ 4];
		m_FrustumPlanes[2][1] = clipMatrix.mArray[13] + clipMatrix.mArray[ 5];
		m_FrustumPlanes[2][2] = clipMatrix.mArray[14] + clipMatrix.mArray[ 6];
		m_FrustumPlanes[2][3] = clipMatrix.mArray[15] + clipMatrix.mArray[ 7];
		/* Normalize the result */
		t = sqrt( m_FrustumPlanes[2][0] * m_FrustumPlanes[2][0] + m_FrustumPlanes[2][1] * m_FrustumPlanes[2][1] + m_FrustumPlanes[2][2]    * m_FrustumPlanes[2][2] );
		m_FrustumPlanes[2][0] /= t;
		m_FrustumPlanes[2][1] /= t;
		m_FrustumPlanes[2][2] /= t;
		m_FrustumPlanes[2][3] /= t;
		/* Extract the TOP plane */
		m_FrustumPlanes[3][0] = clipMatrix.mArray[12] - clipMatrix.mArray[ 4];
		m_FrustumPlanes[3][1] = clipMatrix.mArray[13] - clipMatrix.mArray[ 5];
		m_FrustumPlanes[3][2] = clipMatrix.mArray[14] - clipMatrix.mArray[ 6];
		m_FrustumPlanes[3][3] = clipMatrix.mArray[15] - clipMatrix.mArray[ 7];
		/* Normalize the result */
		t = sqrt( m_FrustumPlanes[3][0] * m_FrustumPlanes[3][0] + m_FrustumPlanes[3][1] * m_FrustumPlanes[3][1] + m_FrustumPlanes[3][2]    * m_FrustumPlanes[3][2] );
		m_FrustumPlanes[3][0] /= t;
		m_FrustumPlanes[3][1] /= t;
		m_FrustumPlanes[3][2] /= t;
		m_FrustumPlanes[3][3] /= t;
		/* Extract the FAR plane */
		m_FrustumPlanes[4][0] = clipMatrix.mArray[12] - clipMatrix.mArray[ 8];
		m_FrustumPlanes[4][1] = clipMatrix.mArray[13] - clipMatrix.mArray[ 9];
		m_FrustumPlanes[4][2] = clipMatrix.mArray[14] - clipMatrix.mArray[10];
		m_FrustumPlanes[4][3] = clipMatrix.mArray[15] - clipMatrix.mArray[11];
		/* Normalize the result */
		t = sqrt( m_FrustumPlanes[4][0] * m_FrustumPlanes[4][0] + m_FrustumPlanes[4][1] * m_FrustumPlanes[4][1] + m_FrustumPlanes[4][2]    * m_FrustumPlanes[4][2] );
		m_FrustumPlanes[4][0] /= t;
		m_FrustumPlanes[4][1] /= t;
		m_FrustumPlanes[4][2] /= t;
		m_FrustumPlanes[4][3] /= t;
		/* Extract the NEAR plane */
		m_FrustumPlanes[5][0] = clipMatrix.mArray[12] + clipMatrix.mArray[ 8];
		m_FrustumPlanes[5][1] = clipMatrix.mArray[13] + clipMatrix.mArray[ 9];
		m_FrustumPlanes[5][2] = clipMatrix.mArray[14] + clipMatrix.mArray[10];
		m_FrustumPlanes[5][3] = clipMatrix.mArray[15] + clipMatrix.mArray[11];
		/* Normalize the result */
		t = sqrt( m_FrustumPlanes[5][0] * m_FrustumPlanes[5][0] + m_FrustumPlanes[5][1] * m_FrustumPlanes[5][1] + m_FrustumPlanes[5][2]    * m_FrustumPlanes[5][2] );
		m_FrustumPlanes[5][0] /= t;
		m_FrustumPlanes[5][1] /= t;
		m_FrustumPlanes[5][2] /= t;
		m_FrustumPlanes[5][3] /= t;
	}


	bool Frustum::IsPointInFrustum(const Vector3f& point)
	{
		int p;
		for( p = 0; p < 6; p++ )
			if( m_FrustumPlanes[p][0] * point.x + m_FrustumPlanes[p][1] * point.y + m_FrustumPlanes[p][2] * point.z + m_FrustumPlanes[p][3]    <= 0 )
				return false;
		return true;
	}

	// AABB���ӽ����ཻ�ж�
	// ժ��ZFXEngine
	bool Frustum::IntersectsAABB(const AABB& aabb)
	{
		Vector3f  vMin, vMax;

		// find and test extreme points
		for (int i=0; i<6; i++) {
			// x coordinate
			if (m_FrustumPlanes[i].n.x <= 0.0f) {
				vMin.x = aabb.worldMin.x;
				vMax.x = aabb.worldMax.x;
			}
			else {
				vMin.x = aabb.worldMax.x;
				vMax.x = aabb.worldMin.x;
			}
			// y coordinate
			if (m_FrustumPlanes[i].n.y <= 0.0f) {
				vMin.y = aabb.worldMin.y;
				vMax.y = aabb.worldMax.y;
			}
			else {
				vMin.y = aabb.worldMax.y;
				vMax.y = aabb.worldMin.y;
			}
			// z coordinate
			if (m_FrustumPlanes[i].n.z <= 0.0f) {
				vMin.z = aabb.worldMin.z;
				vMax.z = aabb.worldMax.z;
			}
			else {
				vMin.z = aabb.worldMax.z;
				vMax.z = aabb.worldMin.z;
			}

			if ( ((m_FrustumPlanes[i].n*vMin) + m_FrustumPlanes[i].d) < 0.0f)
				return false;

			//if ( ((m_FrustumPlanes[i].n*vMax) + m_FrustumPlanes[i].d) >= 0.0f)
			//	return true;
		} // for

		return true;
	}

	// �������ӽ����ڵ����������ü���ľ���
	float Frustum::SphereInFrustum(const Vector3f& point, float radius)
	{
		int p;
		float d;
		for( p = 0; p < 6; p++ )
		{
			d = m_FrustumPlanes[p][0] * point.x + m_FrustumPlanes[p][1] * point.y + m_FrustumPlanes[p][2] * point.z + m_FrustumPlanes[p][3];
			if( d <= -radius )
				return 0;
		}
		return d + radius;
	}
}
