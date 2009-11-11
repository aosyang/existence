#include "BspTriangle.h"

namespace Gen
{
	BspTriangle::BspTriangle()
	: setup(false)
	{
	}

	BspTriangle::BspTriangle(const Vector3f& v0, const Vector3f& v1, const Vector3f& v2)
	: setup(false)
	{
		vertices[0] = v0;
		vertices[1] = v1;
		vertices[2] = v2;

		Setup();
	}

	BspTriangle::BspTriangle(const BspTriangle& triangle)
	: setup(false)
	{
		CloneData(triangle);
	}

	BspTriangle::~BspTriangle()
	{
		DestroyData();
	}

	void BspTriangle::Setup()
	{
		normal = CrossProduct(vertices[1] - vertices[0], vertices[2] - vertices[0]);
		normal.normalize();
		offset = -vertices[0] * normal;

		edgeNormals[0] = CrossProduct(normal, vertices[0] - vertices[2]);
		edgeNormals[1] = CrossProduct(normal, vertices[1] - vertices[0]);
		edgeNormals[2] = CrossProduct(normal, vertices[2] - vertices[1]);

		edgeOffsets[0] = edgeNormals[0] * vertices[0];
		edgeOffsets[1] = edgeNormals[1] * vertices[1];
		edgeOffsets[2] = edgeNormals[2] * vertices[2];

		setup = true;
	}

	void BspTriangle::CloneData(const BspTriangle& triangle)
	{
		//AssertFatal(triangle.setup, "BspTriangle::CloneData() : Target triangle hasn't set up edge infos.");

		normal = triangle.normal;
		offset = triangle.offset;

		for (int i=0; i<3; i++)
		{
			vertices[i] = triangle.vertices[i];
			edgeNormals[i] = triangle.edgeNormals[i];
			edgeOffsets[i] = triangle.edgeOffsets[i];
		}
		setup = triangle.setup;
	}

	void BspTriangle::DestroyData()
	{
		//SAFE_DELETE_ARRAY(vertices);
	}

	bool BspTriangle::IsAbove(const Vector3f& pos) const
	{
		if (!setup) return false;
		return ((edgeNormals[0]*pos>=edgeOffsets[0]) &&
				(edgeNormals[1]*pos>=edgeOffsets[1]) &&
				(edgeNormals[2]*pos>=edgeOffsets[2]));
	}


	BspTriangle& BspTriangle::operator=(const BspTriangle& rhs)
	{
		if (&rhs!=this)
		{
			DestroyData();

			CloneData(rhs);
		}

		return *this;
	}
}
