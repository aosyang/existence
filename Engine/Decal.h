//-----------------------------------------------------------------------------------
/// Decal.h ����
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------

#ifndef _DECAL_H
#define _DECAL_H

#include "BaseLightableObject.h"
#include "Material.h"
#include "IVertexBuffer.h"


//-----------------------------------------------------------------------------------
/// \brief
/// ������ƽ��������������
/// 
/// Write detailed description for Decal here.
/// 
/// \remarks
/// Write remarks for Decal here.
//-----------------------------------------------------------------------------------
class Decal : public BaseLightableObject
{
public:
	Decal();
	~Decal();

	// ----- Overwrite ISceneObject

	void Render();
	//void DebugRender();

	// ----- Overwrite BaseSceneObject

	void PrepareRenderObjects(ChildrenSceneObjectsSet& objects);
	bool IntersectsRay(const Ray& ray, CollisionInfo& info);

	// ----- Decal Methods

	// ����
	inline Material* GetMaterial() const { return m_Material; }
	inline void SetMaterial(Material* mat) { m_Material = mat; }

	void SetDirection(const Vector3f normal);

	void SetSize(float size);
	float GetSize() const { return m_Size; }

private:

	// ���ݳߴ繹�춥��
	void BuildVertexArray();

protected:
	Material*	m_Material;		///< ��������

	//unsigned int	m_FaceArray[3 * 2];
	//float			m_VertexArray[3 * 4];
	//float			m_NormalArray[3 * 4];
	//float			m_TexCoordArray[2 * 4];
	IVertexBuffer*	m_VertexBuffer;

	float			m_Size;
};

#endif