//-----------------------------------------------------------------------------------
/// DistantViewObject.h Զ��������Ⱦ������պС�Զ�����������Ķ���
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

	// ָ������ʹ�õ�����
	void SetMesh(Mesh* mesh);

	// ģ��λ�Ʊ���
	inline void SetOffsetScale(const Vector3f& scale) { m_OffsetScale = scale; }
	inline Vector3f GetOffsetScale() const { return m_OffsetScale; }

	bool RayPicking(const Ray& ray, Vector3f& point, Vector3f& normal, float& d, bool infiniteLength = false);

private:
	Mesh*		m_Mesh;

	Vector3f	m_OffsetScale;
};

#endif