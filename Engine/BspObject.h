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
#include "BaseSceneObject.h"

class BspObject : public BaseSceneObject
{
public:
	BspObject();
	~BspObject();

	// ----- Overwrite ISceneObject

	void Render();
	void DebugRender();

	// ----- Overwrite BaseSceneObject

	int GetCollisionType() const { return COLLISION_TYPE_BSP; }
	void PrepareRenderObjects(SceneObjectList& objects);
	bool IntersectsRay(const Ray& ray, CollisionInfo& info);

	// BspObject Methods

	// 指定用于生成bsp结构的mesh
	void SetMesh(Mesh* mesh);

	bool PushSphere(const Vector3f& pos, Vector3f& newpos, float radius) const;
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
