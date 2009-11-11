#ifndef _IMESH_H
#define _IMESH_H

#include "Matrix4.h"
#include "OBB.h"
#include "MeshElement.h"

namespace Gen
{
	class Material;

	class IMesh
	{
	public:
		virtual ~IMesh() {}

		// mesh element
		virtual int GetElementCount() const = 0;
		virtual MeshElement* GetElement(unsigned int index) = 0;

		virtual Material* GetMaterial(unsigned int index) = 0;

		virtual String GetName() const = 0;
		virtual void SetNamePtr(const String* namePtr) = 0;

		virtual float GetBoundingRadius() const = 0;
		virtual const OBB& GetOBB() const = 0;
	};
}

#endif
