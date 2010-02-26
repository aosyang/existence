#ifndef _VERTEXINFO_H
#define _VERTEXINFO_H

//#define EPSILON 0.00001f
//#define FLOAT_EQUALS_ZERO(num) (fabs(num) <= EPSILON)
//#define FLOAT_EQUAL(a, b) (fabs(a-b) <= EPSILON)

#include <MathUtil.h>

class VertexInfo
{
public:
	float x, y, z;
	float nx, ny, nz;
	float u, v;

	int index;

	bool operator<(const VertexInfo& rhs) const
	{
		return lessThan(rhs);
	}

	bool lessThan(const VertexInfo& rhs, int level=0) const
	{
		float l = (&this->x)[level];
		float r = (&rhs.x)[level];

		if (!FLOAT_EQUAL(l, r) && l < r)
			return true;
		else
		{
			if (FLOAT_EQUAL(l, r) && level<7)
				return lessThan(rhs, level+1);
			else
				return false;
		}
	}

};

#endif
