#ifndef _BSPTRIANGLE_H
#define _BSPTRIANGLE_H

#include "Vector3f.h"
#include "Debug.h"

namespace Gen
{
	class BspTriangle
	{
	public:
		Vector3f normal;
		float offset;

		Vector3f vertices[3];

		Vector3f edgeNormals[3];
		float edgeOffsets[3];

		bool setup;

		BspTriangle();

		BspTriangle(const Vector3f& v0, const Vector3f& v1, const Vector3f& v2);

		BspTriangle(const BspTriangle& triangle);
		~BspTriangle();

		void Setup();

		void CloneData(const BspTriangle& triangle);
		void DestroyData();

		// ����һ�����������ͶӰ�Ƿ�λ�ڶ�����ڲ�
		bool IsAbove(const Vector3f& pos) const;

		BspTriangle& operator=(const BspTriangle& rhs);
	};
}

#endif

