//-----------------------------------------------------------------------------------
/// RenderableObjectBase.h 可渲染对象基类，实现了渲染对象所需要使用的方法
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#ifndef _RENDERABLEOBJECTBASE_H
#define _RENDERABLEOBJECTBASE_H

#include "IRenderableObject.h"
#include "SceneObject.h"

namespace Gen
{
	class LightableObject;

	class RenderableObjectBase : public IRenderableObject,
		public SceneObject
	{
		DECLARE_FACTORY_OBJECT(RenderableObjectBase)
	public:

		// ----- Overwrite SceneObject

		void Destroy();

		// ----- Overwrite IRenderableObject

		void RenderSingleObject();

		// 是否透明
		bool IsTransparent() { return false; }

		// 获取包围盒
		const AABB& GetAABB();

		const OBB& GetOBB();

		void SetRenderOrder(unsigned int order);


		// ----- Overwrite MovableObjectBase

		void CollectRenderableObject(RenderableObjectList& renderableObjs, Frustum* frustum);


		// ----- RenderableObjectBase methods

		// 使物体能够被照亮
		void CreateLightableObject();
		void DestroyLightableObject();

		// 光照用包围球半径
		float GetBoundingRadius() const { return m_BoundingSphereRadius; }

		unsigned int GetRenderOrder() const { return m_RenderOrder; }

	protected:
		bool							m_DebugRender;
		float							m_BoundingSphereRadius;		///< 包围球半径

		bool							m_Visible;					///< 可见性
		unsigned int					m_RenderOrder;

		LightableObject*				m_LightableObject;
	};
}

#endif
