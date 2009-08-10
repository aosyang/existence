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

class Frustum
{
public:
	Frustum();

	// ����͸��ͶӰ����
	Matrix4 BuildPrespectiveProjMatrix(float fovy = 45.0f, float aspect = 4.0f/3.0f, float near = 1.0f, float far = 100.0f);

	// ��������ͶӰ����
	Matrix4 BuildOrthographicProjMatrix(float left, float right, float bottom, float top, float znear, float zfar);

	// ����ͶӰ����
	Matrix4 BuildProjectionMatrix(float left, float right, float bottom, float top, float znear, float zfar);

	// ��ȡ�ӽ���ƽ��
	void ExtractFrustumPlanes(const Matrix4& projMatrix, const Matrix4& viewMatrix);

	// ����-�ӽ��塱���
	bool IsPointInFrustum(const Vector3f& point);

	// ������-�ӽ��塱���
	float SphereInFrustum(const Vector3f& point, float radius);

	Matrix4& ProjectionMatrix() { return m_ProjMatrix; }

	float	m_Left, m_Right, m_Bottom, m_Top;
private:
	Plane3			m_FrustumPlanes[6];			///< �ӽ���ƽ�棬�����ֱ����ϵ��a, b, c, d

	Matrix4			m_ProjMatrix;
};

#endif
