#ifndef _PLANE3_H
#define _PLANE3_H

#include "MathUtil.h"
#include "BspTriangle.h"
#include "Vector3f.h"

#include <vector>

using namespace std;

namespace Gen
{
	//#define EPSILON 0.00001f

	enum PointLoc
	{
		ptFront,
		ptBack,
		ptCoplanar,
	};

	enum PointListLoc
	{
		plistFront,
		plistBack,
		plistSplit,
		plistCoplanar,
	};


	class Plane3
	{
	public:
		Vector3f n;		///< 法线
		float d;		///< 原点沿法线方向到平面的距离

		Plane3(float nx, float ny, float nz, float _d)
			: n(nx, ny , nz), d(_d)
		{
		}

		Plane3(const Vector3f& _n, float _d)
			: n(_n), d(_d)
		{
		}

		// 三点式求得平面
		Plane3(const Vector3f& a, const Vector3f& b, const Vector3f& c);

		// 点法式求得平面
		Plane3(const Vector3f& normal, const Vector3f& p);

		// 多边形求得平面
		// TODO : 完成多边形类先
		Plane3(const BspTriangle& triangle);

		Plane3() {}

		// 翻转法线方向
		void Flip()
		{
			n = -n;
		}

		float& operator[](size_t e) { return (e<3) ? n[e] : d; }

		// 求点在平面的方向
		PointLoc TestPoint(const Vector3f& point) const;

		// 点集与平面的位置关系
		PointListLoc TestPList(const Vector3f* list, int num);

		PointListLoc TestPoly(const BspTriangle& triangle);

		const Vector3f Split(const Vector3f& a, const Vector3f& b) const;

		//bool Clip(const BspTriangle& in, BspTriangle* out) const;

		//bool Split(const BspTriangle& in, BspTriangle* polyFront, BspTriangle* polyBack) const;
		bool Split(const BspTriangle& in, vector<BspTriangle>& trianglesFront, vector<BspTriangle>& trianglesBack) const;
	};
}

#endif
