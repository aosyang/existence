//-----------------------------------------------------------------------------------
/// Ray.cpp 射线
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------

#include "Ray.h"
#include <memory.h>

Ray::Ray()
{
	memset(this, 0, sizeof(Ray));
}

Ray::Ray(const Vector3f& begin, const Vector3f& end)
{
	origin = begin;
	direction = end - begin;
	direction.normalize();

	distance = (end - begin).Length();
}

Ray::Ray(const Vector3f& origin, const Vector3f& direction, float distance)
{
	this->origin = origin;
	this->direction = direction;
	this->distance = distance;
}

Ray& Ray::operator=(const Ray& rhs)
{
	if (this != &rhs)
	{
		memcpy(this, &rhs, sizeof(Ray));
	}

	return *this;
}

bool Ray::IntersectsSphere(const Vector3f& point, float radius) const
{
	// 射线起点与球心的相对坐标
	Vector3f relPos = point - origin;

	// 射线出发点位于球体内部
	if (relPos.SquaredLength() <= radius * radius)
	{
		return true;
	}

	// 如果球心位于射线后方
	if (direction * (point - origin) <= 0)
	{
		return false;
	}
	else
	{
		float a = direction * direction;
		float b = 2 * (relPos * direction);
		float c = relPos * relPos - radius * radius;

		float d = (b * b) - (4 * a * c);

		if (d<0)
		{
			return false;
		}
		else
		{
			float t = (-b - sqrt(d)) / (2 * a);
			if (t<0)
				t = (-b + sqrt(d)) / (2 * a);
			return true;
		}
	}
}

bool Ray::IntersectsTriangle(const Vector3f& v1, const Vector3f& v2, const Vector3f& v3, Vector3f& intersectionPoint, float& d, bool infiniteLen) const
{
	Vector3f e1,e2,p,s,q;
	float u,v,tmp;
	e1 = v2 - v1;
	e2 = v3 - v1;
	p = CrossProduct(direction, e2);

	// 面法线方向
	Vector3f normal = CrossProduct(e1, e2);
	normal.normalize();

	// 面法线方向与射线同向或者垂直，视为检测失败
	// 注：这个方法目前只是部分需要
	if (direction * normal>0)
		return false;

	tmp = p * e1;
	if (FLOAT_EQUALS_ZERO(tmp))
	{
		return false;
	}
	tmp = 1.0f/tmp;
	s = origin - v1;
	u = s * p * tmp;
	if (u<0.0f || u>1.0f)
	{
		return false;
	}
	q = CrossProduct(s,e1);
	v = tmp * (direction*q);
	if (v<0.0f || v>1.0f)
	{
		return false;
	}
	if (u+v>1.0f)
		return false; 

	// 射线起点到交点的距离
	float tmpD = tmp *(e2*q);

	if (!infiniteLen && (tmpD<0.0f || tmpD>distance))
		return false;
	else
	{
		d = tmpD;
		intersectionPoint = origin + direction * d;
		return true;
	}
}

bool Ray::IntersectsBox(const Vector3f& vMin, const Vector3f& vMax, Vector3f& intersectionPoint, float& d, bool infiniteLen) const
{
	bool result = false;

#define VECTOR_LESS(a, b) (a.x<b.x && a.y<b.y && a.z<b.z)
#define VECTOR_GREATER(a, b) (a.x>b.x && a.y>b.y && a.z>b.z)

	// Point inside box
	if (VECTOR_LESS(this->origin, vMax) && VECTOR_GREATER(this->origin, vMin))
	{
		intersectionPoint = this->origin;
		d = 0.0f;

		return true;
	}

	// x+
	if (direction * Vector3f(1.0f, 0.0f, 0.0f)<0)
	{
		result |= IntersectsTriangle(
			Vector3f(vMax.x, vMax.y, vMax.z),
			Vector3f(vMax.x, vMin.y, vMin.z),
			Vector3f(vMax.x, vMax.y, vMin.z),
			intersectionPoint, d, infiniteLen);

		result |= IntersectsTriangle(
			Vector3f(vMax.x, vMax.y, vMax.z),
			Vector3f(vMax.x, vMin.y, vMax.z),
			Vector3f(vMax.x, vMin.y, vMin.z),
			intersectionPoint, d, infiniteLen);

		if (result)
			return true;
	}

	// x-
	if (direction * Vector3f(-1.0f, 0.0f, 0.0f)<0)
	{
		result |= IntersectsTriangle(
			Vector3f(vMin.x, vMax.y, vMax.z),
			Vector3f(vMin.x, vMax.y, vMin.z),
			Vector3f(vMin.x, vMin.y, vMin.z),
			intersectionPoint, d, infiniteLen);

		result |= IntersectsTriangle(
			Vector3f(vMin.x, vMax.y, vMax.z),
			Vector3f(vMin.x, vMin.y, vMin.z),
			Vector3f(vMin.x, vMin.y, vMax.z),
			intersectionPoint, d, infiniteLen);

		if (result)
			return true;
	}

	// z+
	if (direction * Vector3f(0.0f, 0.0f, 1.0f)<0)
	{
		result |= IntersectsTriangle(
			Vector3f(vMax.x, vMax.y, vMax.z),
			Vector3f(vMin.x, vMin.y, vMax.z),
			Vector3f(vMax.x, vMin.y, vMax.z),
			intersectionPoint, d, infiniteLen);

		result |= IntersectsTriangle(
			Vector3f(vMax.x, vMax.y, vMax.z),
			Vector3f(vMin.x, vMax.y, vMax.z),
			Vector3f(vMin.x, vMin.y, vMax.z),
			intersectionPoint, d, infiniteLen);

		if (result)
			return true;
	}

	// z-
	if (direction * Vector3f(0.0f, 0.0f, -1.0f)<0)
	{
		result |= IntersectsTriangle(
			Vector3f(vMax.x, vMax.y, vMin.z),
			Vector3f(vMax.x, vMin.y, vMin.z),
			Vector3f(vMin.x, vMin.y, vMin.z),
			intersectionPoint, d, infiniteLen);

		result |= IntersectsTriangle(
			Vector3f(vMax.x, vMax.y, vMin.z),
			Vector3f(vMin.x, vMin.y, vMin.z),
			Vector3f(vMin.x, vMax.y, vMin.z),
			intersectionPoint, d, infiniteLen);

		if (result)
			return true;
	}


	// y+
	if (direction * Vector3f(0.0f, 1.0f, 0.0f)<0)
	{
		result |= IntersectsTriangle(
			Vector3f(vMax.x, vMax.y, vMax.z),
			Vector3f(vMax.x, vMax.y, vMin.z),
			Vector3f(vMin.x, vMax.y, vMax.z),
			intersectionPoint, d, infiniteLen);

		result |= IntersectsTriangle(
			Vector3f(vMax.x, vMax.y, vMin.z),
			Vector3f(vMin.x, vMax.y, vMin.z),
			Vector3f(vMin.x, vMax.y, vMax.z),
			intersectionPoint, d, infiniteLen);

		if (result)
			return true;
	}

	// y-
	if (direction * Vector3f(0.0f, -1.0f, 0.0f)<0)
	{
		result |= IntersectsTriangle(
			Vector3f(vMax.x, vMin.y, vMax.z),
			Vector3f(vMin.x, vMin.y, vMax.z),
			Vector3f(vMax.x, vMin.y, vMin.z),
			intersectionPoint, d, infiniteLen);

		result |= IntersectsTriangle(
			Vector3f(vMax.x, vMin.y, vMin.z),
			Vector3f(vMin.x, vMin.y, vMax.z),
			Vector3f(vMin.x, vMin.y, vMin.z),
			intersectionPoint, d, infiniteLen);

		if (result)
			return true;
	}

	return false;
}

// 矩阵对射线进行变换
Ray operator*(const Matrix4& mat, const Ray& ray)
{
	Ray r;
	r.origin = mat * ray.origin;
	r.direction = mat.GetRotationMatrix() * ray.direction;
	r.distance = ray.distance;

	return r;
}

