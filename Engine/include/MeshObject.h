//-----------------------------------------------------------------------------------
/// MeshObject.h ������󣬿��Խ�������Դ��Ⱦ����Ļ
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

		// ָ������ʹ�õ�����
		virtual void SetMesh(BaseMesh* mesh);

		// ����
		// TODO: ���ʵ�ָ��������Ҫ�޸�
		// note: �����������Ѿ�û�ã�ʹ�ö�ӦMeshElement�Ĳ����趨����
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
