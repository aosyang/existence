//-----------------------------------------------------------------------------------
/// DistantViewObject.h Զ��������Ⱦ������պС�Զ�����������Ķ���
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 - 2010 by Mwolf
//-----------------------------------------------------------------------------------

#ifndef _DISTANTVIEWOBJECT_H
#define _DISTANTVIEWOBJECT_H

#include "SceneObject.h"
#include "BaseMesh.h"

namespace Gen
{
	class DistantViewObject : public SceneObject
	{
		DECLARE_FACTORY_OBJECT(DistantViewObject, SceneObject);
	public:
		// ----- Overwrite SceneObject

		void RenderSingleObject();

		// ----- Overwrite MovableObjectBase

		bool IsCulled(Frustum* frustum);

		// ----- DistantViewObject Methods

		/// @brief
		///	ָ��Զ������ʹ�õ�ģ��
		/// @param mesh
		///		��Ⱦ�õ�ģ��
		void SetMesh(BaseMesh* mesh);

		// ģ��λ�Ʊ���
		inline void SetOffsetScale(const Vector3f& scale) { m_OffsetScale = scale; }
		inline Vector3f GetOffsetScale() const { return m_OffsetScale; }

	private:
		Vector3f		m_OffsetScale;		///< λ��ƫ������

		BaseMesh*		m_Mesh;				///< ��Ⱦʱʹ�õ�ģ��
	};
}

#endif
