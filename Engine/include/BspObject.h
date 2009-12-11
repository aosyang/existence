//-----------------------------------------------------------------------------------
/// BspObject.h Bsp���󣬴���bsp����Ⱦ����ײ
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------

#ifndef _BSPOBJECT_H
#define _BSPOBJECT_H

#include "BspTree.h"
#include "RenderableObjectBase.h"

namespace Gen
{
	class BspObject : public RenderableObjectBase
	{
		DECLARE_FACTORY_OBJECT(BspObject);
	public:
		// ----- Overwrite IRenderableObject

		void RenderSingleObject();

		// ----- Overwrite SceneObject

		bool IntersectsRay(const Ray& ray, CollisionResult& info);
		bool OnSave(SceneSerializerNode* node);
		void OnRestore(SceneSerializerNode* node);

		// ----- BspObject Methods

		// ָ����������bsp�ṹ��mesh
		void SetMesh(IMesh* mesh);

		// ʹ�����������ײ��newpos������һ���Ϸ���λ��
		bool PushSphere(const Vector3f& pos, Vector3f& newpos, float radius);
		void TraverseTree(
			vector< BspTriangle* >* polyList,
			const Vector3f& loc );
		bool Intersects(const Ray& ray, Vector3f* point, BspTriangle** triangle = NULL);

		bool IsPointInSolid(const Vector3f& point);

	private:
		IMesh*		m_Mesh;

		BspTree*	m_Bsp;
	};
}

#endif

