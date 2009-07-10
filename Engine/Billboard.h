//-----------------------------------------------------------------------------------
/// Billboard.h 公告板对象
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#ifndef _BILLBOARD_H
#define _BILLBOARD_H

#include "BaseLightableObject.h"
#include "Material.h"
#include "IVertexBuffer.h"

class Billboard : public BaseLightableObject
{
public:
	Billboard();
	~Billboard();

	// ----- Overwrite ISceneObject

	//void Update(unsigned long deltaTime);
	void Render();
	//void DebugRender();

	// ----- Overwrite BaseSceneObject

	void PrepareRenderObjects(ChildrenSceneObjectsSet& objects);

	// ----- Billboard Methods

	// 材质
	inline Material* GetMaterial() const { return m_Material; }
	inline void SetMaterial(Material* mat) { m_Material = mat; }

protected:
	Material*	m_Material;
	float		m_Radius;

	//unsigned int	m_FaceArray[3 * 2];
	//float			m_VertexArray[3 * 4];
	//float			m_NormalArray[3 * 4];
	//float			m_TexCoordArray[2 * 4];

	IVertexBuffer*	m_VertexBuffer;
};

#endif
