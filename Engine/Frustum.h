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

class Frustum
{
public:
	Frustum();

	// 生成透视投影矩阵
	Matrix4 BuildPrespectiveProjMatrix(float fovy = 45.0f, float aspect = 4.0f/3.0f, float near = 1.0f, float far = 100.0f);

	// 生成正交投影矩阵
	Matrix4 BuildOrthographicProjMatrix(float left, float right, float bottom, float top, float znear, float zfar);

	// 生成投影矩阵
	Matrix4 BuildProjectionMatrix(float left, float right, float bottom, float top, float znear, float zfar);

	// 提取视截体平面
	void ExtractFrustumPlanes(const Matrix4& projMatrix, const Matrix4& viewMatrix);

	// “点-视截体”检测
	bool IsPointInFrustum(const Vector3f& point);

	// “球体-视截体”检测
	float SphereInFrustum(const Vector3f& point, float radius);

	Matrix4& ProjectionMatrix() { return m_ProjMatrix; }

	float	m_Left, m_Right, m_Bottom, m_Top;
private:
	Plane3			m_FrustumPlanes[6];			///< 视截体平面，参数分别代表系数a, b, c, d

	Matrix4			m_ProjMatrix;
};

#endif
