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
#include "IMesh.h"

namespace Gen
{
	class DistantViewObject : public RenderableObjectBase
	{
		DECLARE_FACTORY_OBJECT(DistantViewObject);
	public:
		// ----- Overwrite IRenderableObject

		void RenderSingleObject();

		// ----- Overwrite MovableObjectBase

		bool IsCulled(Frustum* frustum);

		// ----- DistantViewObject Methods

		// ָ������ʹ�õ�����
		void SetMesh(IMesh* mesh);

		// ģ��λ�Ʊ���
		inline void SetOffsetScale(const Vector3f& scale) { m_OffsetScale = scale; }
		inline Vector3f GetOffsetScale() const { return m_OffsetScale; }

	private:
		IMesh*		m_Mesh;

		Vector3f	m_OffsetScale;
	};
}

#endif
