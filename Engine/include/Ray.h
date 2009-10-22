//-----------------------------------------------------------------------------------
/// Ray.h ����
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#ifndef _RAY_H
#define _RAY_H

#include "Vector3f.h"
#include "MathUtil.h"
#include "Matrix4.h"

// ����
class Ray
{
public:
	Vector3f	origin;			///< ���߷�����
	Vector3f	direction;		///< ���߷���
	float		distance;		///< ���߾���

	// TODO: ���Ա�ʾ�Ƿ���������

	Ray();
	Ray(const Vector3f& begin, const Vector3f& end);
	Ray(const Vector3f& origin, const Vector3f& direction, float distance);

	Ray& operator=(const Ray& rhs);

	bool IntersectsSphere(const Vector3f& point, float radius) const;

	// �������������ཻ
	//  d ����������㵽����ľ���
	bool IntersectsTriangle(const Vector3f& v1, const Vector3f& v2, const Vector3f& v3, Vector3f& intersectionPoint, float& d, bool infiniteLen = false) const;

	bool IntersectsBox(const Vector3f& vMin, const Vector3f& vMax, Vector3f& intersectionPoint, float& d, bool infiniteLen = false) const;
};

Ray operator*(const Matrix4& mat, const Ray& ray);

#endif
