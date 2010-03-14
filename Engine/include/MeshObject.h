//-----------------------------------------------------------------------------------
/// MeshObject.h 网格对象，可以将网格资源渲染到屏幕
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------

#ifndef _MESHOBJECT_H
#define _MESHOBJECT_H

#include "RenderableObjectBase.h"
#include "EmdMesh.h"
#include "Material.h"

namespace Gen
{
	class BaseMesh;

	class MeshObject : public RenderableObjectBase
	{
		DECLARE_FACTORY_OBJECT(MeshObject, RenderableObjectBase);
	public:
		// ----- Overwrite SceneObject
		bool OnSave(SceneSerializerNode* node);
		void OnRestore(SceneSerializerNode* node);

		// ----- Overwrite IRenderableObject

		void RenderSingleObject();

		// ----- MeshObject Methods

		// 指定对象使用的网格
		virtual void SetMesh(BaseMesh* mesh);

		// 材质
		// TODO: 材质的指定方法需要修改
		// note: 这两个方法已经没用，使用对应MeshElement的材质设定方法
		inline int GetSubMeshCount() const { return m_SubMeshCount; }
		inline Material* GetMaterial(int index) const { return m_Materials[index]; }
		void SetMaterial(Material* mat, int index);

		//void SetFrustumCulling(bool cull) { m_FrustumCulling = cull; }
		//bool GetFrustumCulling() const { return m_FrustumCulling; }

		bool RayPicking(const Ray& ray, Vector3f& point, Vector3f& normal, float& d, bool infiniteLength = false);

		//bool RayPickingTriangle(const Ray& ray);
	protected:
		BaseMesh*		m_Mesh;

		int			m_SubMeshCount;
		Material**	m_Materials;

		//bool		m_FrustumCulling;
	};
}

#endif
