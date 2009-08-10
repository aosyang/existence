//-----------------------------------------------------------------------------------
/// MeshObject.h ������󣬿��Խ�������Դ��Ⱦ����Ļ
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------

#ifndef _MESHOBJECT_H
#define _MESHOBJECT_H

#include "BaseLightableObject.h"
#include "Mesh.h"
#include "Material.h"

class Mesh;

class MeshObject : public BaseLightableObject
{
public:
	MeshObject();
	~MeshObject();

	// ----- Overwrite ISceneObject

	void Update(unsigned long deltaTime);
	void Render();
	void DebugRender();

	// ----- Overwrite BaseSceneObject

	int GetCollisionType() const { return COLLISION_TYPE_MESH; }
	void PrepareRenderObjects(SceneObjectList& objects);
	bool IntersectsRay(const Ray& ray, CollisionInfo& info);

	// ----- MeshObject Methods

	// ָ������ʹ�õ�����
	void SetMesh(Mesh* mesh);

	// ����
	// TODO: ���ʵ�ָ��������Ҫ�޸�
	// note: �����������Ѿ�û�ã�ʹ�ö�ӦMeshElement�Ĳ����趨����
	inline int GetSubMeshCount() const { return m_SubMeshCount; }
	inline Material* GetMaterial(int index) const { return m_Materials[index]; }
	inline void SetMaterial(Material* mat, int index) { m_Materials[index] = mat; }

	void SetFrustumCulling(bool cull) { m_FrustumCulling = cull; }
	bool GetFrustumCulling() const { return m_FrustumCulling; }

	bool RayPicking(const Ray& ray, Vector3f& point, Vector3f& normal, float& d, bool infiniteLength = false);

private:
	Mesh*		m_Mesh;

	int			m_SubMeshCount;
	Material**	m_Materials;

	bool		m_FrustumCulling;
};

#endif
