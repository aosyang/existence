#ifndef _IMESH_H
#define _IMESH_H

#include "Matrix4.h"

class IMesh
{
public:
	virtual ~IMesh() {}

	virtual void RenderMesh(Matrix4* transform) const = 0;

};

#endif
