//-----------------------------------------------------------------------------------
/// OBB.h Oriented Bounding Box
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#ifndef _OBB_H
#define _OBB_H

#include "Vector3f.h"
#include "Ray.h"


inline Vector3f GetNormal(const Vector3f& v1, const Vector3f& v2, const Vector3f& v3)
{
	Vector3f normal = CrossProduct(v2 - v1, v3 - v1);
	normal.normalize();
	return normal;
}


class OBB
{
public:
	Vector3f localMin, localMax;

	OBB()
	{
		Reset();
	}

	OBB(const Vector3f& p)
	{
		SetPoint(p);
	}

	void SetPoint(const Vector3f& p)
	{
		localMin = localMax = p;
	}

	// 需要时扩展OBB的范围
	void Expand(const Vector3f& p)
	{
		if (p.x > localMax.x)
			localMax.x = p.x;
		if (p.y > localMax.y)
			localMax.y = p.y;
		if (p.z > localMax.z)
			localMax.z = p.z;

		if (p.x < localMin.x)
			localMin.x = p.x;
		if (p.y < localMin.y)
			localMin.y = p.y;
		if (p.z < localMin.z)
			localMin.z = p.z;
	}

	// 通过另一个obb扩展范围
	void Expand(const OBB& obb)
	{
		if (obb.localMax.x > localMax.x)
			localMax.x = obb.localMax.x;
		if (obb.localMax.y > localMax.y)
			localMax.y = obb.localMax.y;
		if (obb.localMax.z > localMax.z)
			localMax.z = obb.localMax.z;

		if (obb.localMin.x < localMin.x)
			localMin.x = obb.localMin.x;
		if (obb.localMin.y < localMin.y)
			localMin.y = obb.localMin.y;
		if (obb.localMin.z < localMin.z)
			localMin.z = obb.localMin.z;
	}

	// 给定一个较大的范围
	void Reset()
	{
		localMin = Vector3f(999999.0f, 999999.0f, 999999.0f);
		localMax = Vector3f(-999999.0f, -999999.0f, -999999.0f);
	}

#define CHECK_TRIANGLE(p1, p2, p3) if (ray.IntersectsTriangle(p1, p2, p3, point, d, infiniteLen)) { normal = GetNormal(p1, p2, p3); return true; }


	bool IntersectsRay(const Ray& ray, Vector3f& point, Vector3f& normal, bool infiniteLen = false)
	{
		// box的各个顶点
		Vector3f
		v1(localMin.x, localMin.y, localMin.z),
		v2(localMin.x, localMax.y, localMin.z),
		v3(localMin.x, localMax.y, localMax.z),
		v4(localMin.x, localMin.y, localMax.z),
		v5(localMax.x, localMin.y, localMin.z),
		v6(localMax.x, localMax.y, localMin.z),
		v7(localMax.x, localMax.y, localMax.z),
		v8(localMax.x, localMin.y, localMax.z);

		float d; 

		CHECK_TRIANGLE(v1, v2, v3)
	}
};

#endif
