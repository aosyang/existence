//-----------------------------------------------------------------------------------
/// MeshObject.h ������󣬿��Խ�������Դ��Ⱦ����Ļ
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------

#ifndef _MESHOBJECT_H
#define _MESHOBJECT_H

#include "GameObject.h"
#include "EmdMesh.h"
#include "Material.h"

namespace Gen
{
	class BaseMesh;

	class MeshObject : public GameObject
	{
		DECLARE_FACTORY_OBJECT(MeshObject, GameObject);
	public:
		// ----- Overwrite SceneObject
		bool OnSave(SceneSerializerNode* node);
		void OnRestore(SceneSerializerNode* node);

		// ----- MeshObject Methods

		void SetMesh(const String& meshName);

		// ָ������ʹ�õ�����
		virtual void SetMesh(BaseMesh* mesh);

		// ����
		// TODO: ���ʵ�ָ��������Ҫ�޸�
		// note: �����������Ѿ�û�ã�ʹ�ö�ӦMeshElement�Ĳ����趨����
		inline int GetSubMeshCount() const { return m_SubMeshCount; }
		inline BaseMaterial* GetMaterial(int index) const { return m_Materials[index]; }

		void SetMaterial(const String& matName, int subMeshIndex);
		void SetMaterial(BaseMaterial* mat, int subMeshIndex);

		//void SetFrustumCulling(bool cull) { m_FrustumCulling = cull; }
		//bool GetFrustumCulling() const { return m_FrustumCulling; }

		bool RayPicking(const Ray& ray, Vector3f& point, Vector3f& normal, float& d, bool infiniteLength = false);

		//bool RayPickingTriangle(const Ray& ray);
	protected:
		// ----- Overwrite SceneObject
		void RenderSingleObject();


	protected:
		BaseMesh*		m_Mesh;

		int				m_SubMeshCount;
		BaseMaterial**	m_Materials;

		//bool			m_FrustumCulling;
	};
}

#endif
