//-----------------------------------------------------------------------------------
/// DistantViewObject.h 远景对象，渲染诸如天空盒、远景景物等物体的对象
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------

#ifndef _DISTANTVIEWOBJECT_H
#define _DISTANTVIEWOBJECT_H

#include "BaseSceneObject.h"
#include "Mesh.h"

class DistantViewObject : public BaseSceneObject
{
public:
	DistantViewObject();
	~DistantViewObject();

	// ----- Overwrite ISceneObject

	void Update(unsigned long deltaTime);
	void Render();
	void DebugRender();

	// ----- Overwrite BaseSceneObject

	void PrepareRenderObjects(ChildrenSceneObjectsSet& objects);
	bool IntersectsRay(const Ray& ray, CollisionInfo& info, int type);

	// ----- DistantViewObject Methods

	// 指定对象使用的网格
	void SetMesh(Mesh* mesh);

	// 模型位移比例
	inline void SetOffsetScale(const Vector3f& scale) { m_OffsetScale = scale; }
	inline Vector3f GetOffsetScale() const { return m_OffsetScale; }

	bool RayPicking(const Ray& ray, Vector3f& point, Vector3f& normal, float& d, bool infiniteLength = false);

private:
	Mesh*		m_Mesh;

	Vector3f	m_OffsetScale;
};

#endif