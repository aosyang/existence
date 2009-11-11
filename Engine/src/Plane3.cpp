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
	//// TODO : ������Ҫ�޸�
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
	//			// ��Ӷ���
	//			out->vertices[out->nElem++] = in.vertices[thisInd];
	//		}
	//
	//		if ((thisRes==ptBack && nextRes==ptFront) ||
	//			(thisRes==ptFront && nextRes==ptBack))
	//		{
	//			// ����и��
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

	// TODO : ������Ҫ�޸�
	//bool Plane3::Split(const BspTriangle& in, BspTriangle* polyFront, BspTriangle* polyBack) const
	bool Plane3::Split(const BspTriangle& in, vector<BspTriangle>& trianglesFront, vector<BspTriangle>& trianglesBack) const
	{
		// ��ƽ�潫һ���������и���ֵ�������Ӧ���ӽڵ���
		// �ָ������������������
		// 1�����߸�һ��������(�и��͸õ�ĶԱ�)
		// 2��һ����������һ��һ��(�и�������)
		// 3��һ����һ������һ��û��(�����ε�һ����λ��ƽ����)
		// 4�����߸�һ��(��������ƽ����)

		// ��˳�����������㹹�ɵ��߶��Ƿ�ƽ���и�

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
				// ������ӵ�����
				//polyFront->vertices[polyFront->nElem++] = in.vertices[thisInd];
				vFront[iFront++] = in.vertices[thisInd];
			}

			if (thisRes==ptBack)
			{
				// ������ӵ�����
				//polyBack->vertices[polyBack->nElem++] = in.vertices[thisInd];
				vBack[iBack++] = in.vertices[thisInd];
			}

			if (thisRes==ptCoplanar)
			{
				// ����������ӵ�
				//polyFront->vertices[polyFront->nElem++] = in.vertices[thisInd];
				//polyBack->vertices[polyBack->nElem++] = in.vertices[thisInd];
				vFront[iFront++] = in.vertices[thisInd];
				vBack[iBack++] = in.vertices[thisInd];
			}

			// ����߶α�ƽ���и�
			if ((thisRes==ptBack && nextRes==ptFront) ||
				(thisRes==ptFront && nextRes==ptBack))
			{
				// ���ָ����ӵ���������
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
