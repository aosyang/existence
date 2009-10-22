//-----------------------------------------------------------------------------------
/// BspObject.h Bsp对象，处理bsp的渲染和碰撞
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------

#ifndef _BSPOBJECT_H
#define _BSPOBJECT_H

#include "BspTree.h"
#include "RenderableObjectBase.h"

class BspObject : public RenderableObjectBase
{
	DECLARE_FACTORY(BspObject);
public:
	BspObject();
	~BspObject();

	// ----- Overwrite SceneObject

	const String GetTypeName() const { return "BspObject"; }

	// ----- Overwrite IRenderableObject

	void RenderSingleObject();

	// ----- Overwrite SceneObject

	bool IntersectsRay(const Ray& ray, CollisionResult& info);

	// ----- BspObject Methods

	// 指定用于生成bsp结构的mesh
	void SetMesh(Mesh* mesh);

	// 使用球体进行碰撞，newpos将返回一个合法的位置
	bool PushSphere(const Vector3f& pos, Vector3f& newpos, float radius);
	void TraverseTree(
		vector< BspTriangle* >* polyList,
		const Vector3f& loc );
	bool Intersects(const Ray& ray, Vector3f* point, BspTriangle** triangle = NULL);

	bool IsPointInSolid(const Vector3f& point);

private:
	Mesh*		m_Mesh;

	BspTree*	m_Bsp;
};

#endif
