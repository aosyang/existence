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
		Vector3f n;		///< ����
		float d;		///< ԭ���ط��߷���ƽ��ľ���

		/// @brief
		/// ���캯��
		/// @param nx
		///		����x����
		/// @param ny
		///		����y����
		///	@param nz
		///		����z����
		/// @param _d
		///		ԭ���ط��߷���ƽ��ľ���
		Plane3(float nx, float ny, float nz, float _d)
			: n(nx, ny , nz), d(_d)
		{
		}

		/// @brief
		/// ���캯��
		/// @param _n
		///		��������
		/// @param _d
		///		ԭ���ط��߷���ƽ��ľ���
		Plane3(const Vector3f& _n, float _d)
			: n(_n), d(_d)
		{
		}

		// ����ʽ���ƽ��

		/// @brief
		/// ���캯��
		/// @par
		///		����ʽ����ƽ�淽��
		/// @param a
		///		����1
		/// @param b
		///		����2
		/// @param c
		///		����3
		Plane3(const Vector3f& a, const Vector3f& b, const Vector3f& c);

		// �㷨ʽ���ƽ��

		/// @brief
		/// ���캯��
		/// @par
		///		�㷨ʽ����ƽ�淽��
		/// @param normal
		///		��������
		/// @param p
		///		ƽ����һ��
		Plane3(const Vector3f& normal, const Vector3f& p);

		// ��������ƽ��
		// TODO : ��ɶ��������
		Plane3(const BspTriangle& triangle);

		Plane3() {}

		// ��ת���߷���
		void Flip()
		{
			n = -n;
		}

		float& operator[](size_t e) { return (e<3) ? n[e] : d; }

		// �����ƽ��ķ���
		PointLoc TestPoint(const Vector3f& point) const;

		// �㼯��ƽ���λ�ù�ϵ
		PointListLoc TestPList(const Vector3f* list, int num);

		PointListLoc TestPoly(const BspTriangle& triangle);

		const Vector3f Split(const Vector3f& a, const Vector3f& b) const;

		//bool Clip(const BspTriangle& in, BspTriangle* out) const;

		//bool Split(const BspTriangle& in, BspTriangle* polyFront, BspTriangle* polyBack) const;
		bool Split(const BspTriangle& in, vector<BspTriangle>& trianglesFront, vector<BspTriangle>& trianglesBack) const;
	};
}

#endif
