//-----------------------------------------------------------------------------------
/// RenderableObjectBase.h ����Ⱦ������࣬ʵ������Ⱦ��������Ҫʹ�õķ���
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

		// �Ƿ�͸��
		bool IsTransparent() { return false; }

		// ��ȡ��Χ��
		const AABB& GetAABB();

		const OBB& GetOBB();

		void SetRenderOrder(unsigned int order);


		// ----- Overwrite MovableObjectBase

		void CollectRenderableObject(RenderableObjectList& renderableObjs, Frustum* frustum);


		// ----- RenderableObjectBase methods

		// ʹ�����ܹ�������
		void CreateLightableObject();
		void DestroyLightableObject();

		// �����ð�Χ��뾶
		float GetBoundingRadius() const { return m_BoundingSphereRadius; }

		unsigned int GetRenderOrder() const { return m_RenderOrder; }

	protected:
		bool							m_DebugRender;
		float							m_BoundingSphereRadius;		///< ��Χ��뾶

		bool							m_Visible;					///< �ɼ���
		unsigned int					m_RenderOrder;

		LightableObject*				m_LightableObject;
	};
}

#endif
