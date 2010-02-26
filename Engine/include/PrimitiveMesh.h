//-----------------------------------------------------------------------------------
/// PrimitiveMesh.h 基础几何体类，可以创建简单几何体
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#ifndef _PRIMITIVEMESH_H
#define _PRIMITIVEMESH_H

#include "BaseMesh.h"

namespace Gen
{
	class PrimitiveMesh : public BaseMesh
	{
	public:
		PrimitiveMesh(const String& filename);

		// ----- PrimitiveMesh Methods

		void CreateBox(float side);

		void CreatePositiveYPlane(float side);
		void CreatePositiveZPlane(float side);

		String GetFileName() const { return ""; }

	protected:
		// ----- Overwrite Resource
		bool LoadImpl();

		// ----- PrimitiveMesh Methods
	};
}

#endif
