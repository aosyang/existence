#ifndef _VERTEXINFO_H
#define _VERTEXINFO_H

#include <math.h>

bool fequal(float a, float b, float e = 0.0001f)
{
	return (fabs(a - b) < e);
}

struct VertexInfo
{
	float x, y, z;
	float nx, ny, nz;
	float u, v;

	bool operator == (const VertexInfo& rhs)
	{
		return ( fequal(x, rhs.x) && fequal( y , rhs.y ) && fequal( z , rhs.z ) &&
			fequal(nx , rhs.nx ) && fequal( ny , rhs.ny ) && fequal( nz , rhs.nz ) &&
			fequal(u , rhs.u ) && fequal( v , rhs.v) );
	}

	VertexInfo& operator= (const VertexInfo& rhs)
	{
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;

		nx = rhs.nx;
		ny = rhs.ny;
		nz = rhs.nz;

		u = rhs.u;
		v = rhs.v;

		return (*this);
	}

	// TODO: 添加其他的顶点信息
};

#endif
