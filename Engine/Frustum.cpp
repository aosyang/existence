//-----------------------------------------------------------------------------------
/// Frustum.cpp 平头视截体
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#include "Frustum.h"

#include "MathUtil.h"

// TODO: ResizeWindow不调整Frustum的aspect ratio

Frustum::Frustum()
//: m_Near(1.0f), m_Far(100.0f),
//  m_FOVy(45.0f), m_Aspect(4.0f/3.0f)
{
}

Matrix4 Frustum::BuildPrespectiveProjMatrix(float fovy, float aspect, float near, float far)
{
	float range = near * tan(DEG_TO_RAD(fovy / 2));

	m_ProjMatrix = BuildProjectionMatrix(-range * aspect, range * aspect, -range, range, near, far);
	return m_ProjMatrix;
}

Matrix4 Frustum::BuildOrthographicProjMatrix(float left, float right, float bottom, float top, float znear, float zfar)
{
	float m00 = 2.0f / (right - left);
	float m11 = 2.0f / (top - bottom);
	float m22 = -2.0f / (zfar - znear);

	float tx = -(right + left) / (right - left);
	float ty = -(top + bottom) / (top - bottom);
	float tz = -(zfar + znear) / (zfar - znear);

	return Matrix4(m00, 0.0f, 0.0f, tx,
				   0.0f, m11, 0.0f, ty,
				   0.0f, 0.0f, m22, tz,
				   0.0f, 0.0f, 0.0f, -1.0f);
}


Matrix4 Frustum::BuildProjectionMatrix(float left, float right, float bottom, float top, float znear, float zfar)
{
	// 保存这些值，方便计算视截体的范围
	m_Left = left;
	m_Right = right;
	m_Bottom = bottom;
	m_Top = top;
	//m_Near = znear;
	//m_Far = zfar;

	float m00 = 2 * znear / (right - left);
	float m11 = 2 * znear / (top - bottom);

	float m02 = (right + left) / (right - left);
	float m12 = (top + bottom) / (top - bottom);
	float m22 = -(zfar + znear) / (zfar - znear);
	float m23 = - 2 * zfar * znear / (zfar - znear);

	return Matrix4(m00,  0.0f, m02,  0.0f,
				   0.0f, m11,  m12,  0.0f,
				   0.0f, 0.0f, m22,  m23,
				   0.0f, 0.0f,-1.0f, 0.0f);
}

// 这些代码从这里复制过来：http://www.racer.nl/reference/vfc_markmorley.htm

void Frustum::ExtractFrustumPlanes(const Matrix4& projMatrix, const Matrix4& viewMatrix)
{
	Matrix4 clipMatrix;
	float t;

	clipMatrix = projMatrix * viewMatrix;

	// 注意这里矩阵数组访问方式与GL方式不一样，需要调整一下

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

// 返回在视截体内的球体距离近裁剪面的距离
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

