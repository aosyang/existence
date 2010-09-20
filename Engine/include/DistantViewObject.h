//-----------------------------------------------------------------------------------
/// DistantViewObject.h 远景对象，渲染诸如天空盒、远景景物等物体的对象
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
		///	指定远景对象使用的模型
		/// @param mesh
		///		渲染用的模型
		void SetMesh(BaseMesh* mesh);

		// 模型位移比例
		inline void SetOffsetScale(const Vector3f& scale) { m_OffsetScale = scale; }
		inline Vector3f GetOffsetScale() const { return m_OffsetScale; }

	private:
		Vector3f		m_OffsetScale;		///< 位移偏量比例

		BaseMesh*		m_Mesh;				///< 渲染时使用的模型
	};
}

#endif
