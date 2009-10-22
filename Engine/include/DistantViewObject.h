//-----------------------------------------------------------------------------------
/// DistantViewObject.h Զ��������Ⱦ������պС�Զ�����������Ķ���
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------

#ifndef _DISTANTVIEWOBJECT_H
#define _DISTANTVIEWOBJECT_H

#include "RenderableObjectBase.h"
#include "Mesh.h"

class DistantViewObject : public RenderableObjectBase
{
	DECLARE_FACTORY(DistantViewObject);
public:
	DistantViewObject();
	~DistantViewObject();

	// ----- Overwrite SceneObject

	const String GetTypeName() const { return "DistantViewObject"; }


	// ----- Overwrite IRenderableObject

	void RenderSingleObject();

	// ----- Overwrite MovableObjectBase

	bool IsCulled(Frustum* frustum);

	// ----- DistantViewObject Methods

	// ָ������ʹ�õ�����
	void SetMesh(Mesh* mesh);

	// ģ��λ�Ʊ���
	inline void SetOffsetScale(const Vector3f& scale) { m_OffsetScale = scale; }
	inline Vector3f GetOffsetScale() const { return m_OffsetScale; }

private:
	Mesh*		m_Mesh;

	Vector3f	m_OffsetScale;
};

#endif
