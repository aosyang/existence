#ifndef _AABB_H
#define _AABB_H

#include "Vector3f.h"
#include "Matrix4.h"
#include "OBB.h"

class AABB
{
public:
	Vector3f worldMax, worldMin;	///< AABB在世界空间中的坐标范围

	AABB()
	{
		Reset();
	}

	AABB(const Vector3f& _vMin, const Vector3f& _vMax)
	{
		worldMax = _vMax;
		worldMin = _vMin;
	}

	void SetPoint(const Vector3f& p)
	{
		worldMin = worldMax = p;
	}

	void Expand(const OBB& obb, const Matrix4& worldTransform)
	{
		Expand(worldTransform * Vector3f(obb.localMax.x, obb.localMax.y, obb.localMax.z));
		Expand(worldTransform * Vector3f(obb.localMax.x, obb.localMax.y, obb.localMin.z));
		Expand(worldTransform * Vector3f(obb.localMax.x, obb.localMin.y, obb.localMax.z));
		Expand(worldTransform * Vector3f(obb.localMax.x, obb.localMin.y, obb.localMin.z));

		Expand(worldTransform * Vector3f(obb.localMin.x, obb.localMax.y, obb.localMax.z));
		Expand(worldTransform * Vector3f(obb.localMin.x, obb.localMax.y, obb.localMin.z));
		Expand(worldTransform * Vector3f(obb.localMin.x, obb.localMin.y, obb.localMax.z));
		Expand(worldTransform * Vector3f(obb.localMin.x, obb.localMin.y, obb.localMin.z));
	}

	void Expand(const Vector3f& p)
	{
		if (p.x > worldMax.x)
			worldMax.x = p.x;
		if (p.y > worldMax.y)
			worldMax.y = p.y;
		if (p.z > worldMax.z)
			worldMax.z = p.z;

		if (p.x < worldMin.x)
			worldMin.x = p.x;
		if (p.y < worldMin.y)
			worldMin.y = p.y;
		if (p.z < worldMin.z)
			worldMin.z = p.z;
	}

	void Expand(const AABB& aabb)
	{
		if (aabb.worldMax.x > worldMax.x)
			worldMax.x = aabb.worldMax.x;
		if (aabb.worldMax.y > worldMax.y)
			worldMax.y = aabb.worldMax.y;
		if (aabb.worldMax.z > worldMax.z)
			worldMax.z = aabb.worldMax.z;

		if (aabb.worldMin.x < worldMin.x)
			worldMin.x = aabb.worldMin.x;
		if (aabb.worldMin.y < worldMin.y)
			worldMin.y = aabb.worldMin.y;
		if (aabb.worldMin.z < worldMin.z)
			worldMin.z = aabb.worldMin.z;
	}

	void Reset()
	{
		worldMin = Vector3f(999999.0f, 999999.0f, 999999.0f);
		worldMax = Vector3f(-999999.0f, -999999.0f, -999999.0f);
	}
};

#endif
