#include "Plane3.h"

namespace Gen
{
	Plane3::Plane3(
		const Vector3f& a,
		const Vector3f& b,
		const Vector3f& c )
	{
		n = CrossProduct(b-a, c-a);
		n.normalize();
		d = -(n*a);
	}

	Plane3::Plane3(const Vector3f &normal, const Vector3f &p)
		: n(normal), d(-(normal*p))
	{

	}

	/*inline*/ Plane3::Plane3( const BspTriangle& triangle )
	{
		Vector3f a = triangle.vertices[0];
		Vector3f b = triangle.vertices[1];
		Vector3f c = triangle.vertices[2];

		n = CrossProduct(b-a, c-a);
		n.normalize();
		d = -(n*a);
	}

	PointLoc Plane3::TestPoint(const Vector3f& point) const
	{
		float dp = (point * n) + d;
		if (dp > EPSILON)
			return ptFront;

		if (dp < -EPSILON)
			return ptBack;

		return ptCoplanar;
	}

	PointListLoc Plane3::TestPList(const Vector3f* list, int num)
	{
		bool allfront = true, allback = true;

		PointLoc res;

		for (int i=0; i<num; i++)
		{
			res = TestPoint(list[i]);

			if (res==ptBack)
				allfront = false;
			else if (res==ptFront)
				allback = false;
		}

		if (allfront && !allback)
			return plistFront;
		else if (!allfront && allback)
			return plistBack;
		else if (!allfront && !allback)
			return plistSplit;

		return plistCoplanar;

	}

	PointListLoc Plane3::TestPoly(const BspTriangle& triangle)
	{
		return TestPList(triangle.vertices, 3);
	}


	const Vector3f Plane3::Split(const Vector3f& a, const Vector3f& b) const
	{
		float aDot = a * n;
		float bDot = b * n;

		float scale = (-d - aDot) / (bDot - aDot);

		return a + (scale * (b - a));
	}
	//
	//
	//// TODO : 这里需要修改
	//bool Plane3::Clip(const BspTriangle& in, BspTriangle* out) const
	//{
	//	if (!out) return false;
	//
	//	int thisInd = 2;
	//	int nextInd = 0;
	//
	//	PointLoc thisRes = TestPoint(in.vertices[thisInd]);
	//	PointLoc nextRes;
	//
	//	for (nextInd=0; nextInd<3; nextInd++)
	//	{
	//		nextRes = TestPoint(in.vertices[nextInd]);
	//
	//		if (thisRes==ptFront || thisRes==ptCoplanar)
	//		{
	//			// 添加顶点
	//			out->vertices[out->nElem++] = in.vertices[thisInd];
	//		}
	//
	//		if ((thisRes==ptBack && nextRes==ptFront) ||
	//			(thisRes==ptFront && nextRes==ptBack))
	//		{
	//			// 添加切割点
	//			out->vertices[out->nElem++] = Split(
	//											 in.vertices[thisInd],
	//											 in.vertices[nextInd] );
	//		}
	//
	//		thisInd = nextInd;
	//		thisRes = nextRes;
	//	}
	//
	//	if (out->nElem>=3)
	//		return true;
	//	return false;
	//}

	// TODO : 这里需要修改
	//bool Plane3::Split(const BspTriangle& in, BspTriangle* polyFront, BspTriangle* polyBack) const
	bool Plane3::Split(const BspTriangle& in, vector<BspTriangle>& trianglesFront, vector<BspTriangle>& trianglesBack) const
	{
		// 以平面将一个三角形切割，并分到各个对应的子节点中
		// 分割结果可能是以下情况：
		// 1、两边各一个三角形(切割顶点和该点的对边)
		// 2、一边两个，另一边一个(切割两条边)
		// 3、一边有一个，另一边没有(三角形的一条边位于平面上)
		// 4、两边各一个(三角形在平面上)

		// 按顺序检测相邻两点构成的线段是否被平面切割

		Vector3f vFront[4];
		Vector3f vBack[4];

		int iFront = 0;
		int iBack = 0;

		int thisInd = 2;
		int nextInd = 0;

		PointLoc thisRes = TestPoint(in.vertices[thisInd]);
		PointLoc nextRes;

		for (nextInd=0; nextInd<3; nextInd++)
		{
			nextRes = TestPoint(in.vertices[nextInd]);

			if (thisRes==ptFront)
			{
				// 将点添加到正面
				//polyFront->vertices[polyFront->nElem++] = in.vertices[thisInd];
				vFront[iFront++] = in.vertices[thisInd];
			}

			if (thisRes==ptBack)
			{
				// 将点添加到背面
				//polyBack->vertices[polyBack->nElem++] = in.vertices[thisInd];
				vBack[iBack++] = in.vertices[thisInd];
			}

			if (thisRes==ptCoplanar)
			{
				// 两个方向都添加点
				//polyFront->vertices[polyFront->nElem++] = in.vertices[thisInd];
				//polyBack->vertices[polyBack->nElem++] = in.vertices[thisInd];
				vFront[iFront++] = in.vertices[thisInd];
				vBack[iBack++] = in.vertices[thisInd];
			}

			// 如果线段被平面切割
			if ((thisRes==ptBack && nextRes==ptFront) ||
				(thisRes==ptFront && nextRes==ptBack))
			{
				// 将分割点添加到两个方向
				Vector3f split = Split(
					in.vertices[thisInd],
					in.vertices[nextInd] );
				//polyFront->vertices[polyFront->nElem++] = split;
				//polyBack->vertices[polyBack->nElem++] = split;
				vFront[iFront++] = split;
				vBack[iBack++] = split;
			}

			thisInd = nextInd;
			thisRes = nextRes;
		}

		if (iFront>=3)
			trianglesFront.push_back(BspTriangle(vFront[0], vFront[1], vFront[2]));

		if (iBack>=3)
			trianglesBack.push_back(BspTriangle(vBack[0], vBack[1], vBack[2]));

		if (iFront==4)
		{
			trianglesFront.push_back(BspTriangle(vFront[0], vFront[2], vFront[3]));
		}
		else if (iBack==4)
		{
			trianglesBack.push_back(BspTriangle(vBack[0], vBack[2], vBack[3]));
		}

		//if (polyFront->nElem>2 && polyBack->nElem>2)
		//{
		//	return true;
		//}

		//return false;

		return true;
	}
}
