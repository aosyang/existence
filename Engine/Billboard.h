//-----------------------------------------------------------------------------------
/// Billboard.h ��������
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

	void Update(unsigned long deltaTime);
	void Render();
	//void DebugRender();

	// ----- Overwrite BaseSceneObject

	//int GetCollisionType() const { return COLLISION_TYPE_BILLBOARD; }
	// ----- Billboard Methods

	// ����
	inline Material* GetMaterial() const { return m_Material; }
	inline void SetMaterial(Material* mat) { m_Material = mat; }

	void SetColor(const Color4f& color);

	// ָ��������Z������ת��(�ӵ㷽��ȥ����ת)
	void SetZRotation(float rad_angle);

	// ָ��Z������ԭ���ƫ��(��ֹ���ı��������嵲ס)
	// TODO FIXME: ƫ�ƻ����ӵ㷽��任���ƶ�
	void SetZOffset(float offset);

	// ָ�������ĳߴ�
	void SetScale(float scale);

	// ָ���������Ļ�ռ�����
	void SetScreenSpaceScale(float scale_x, float scale_y);

	void UpdateVertexData();

protected:
	Material*	m_Material;
	float		m_Radius;

	//unsigned int	m_FaceArray[3 * 2];
	//float			m_VertexArray[3 * 4];
	//float			m_NormalArray[3 * 4];
	//float			m_TexCoordArray[2 * 4];

	IVertexBuffer*	m_VertexBuffer;

	float			m_ZRotataion;
	float			m_ZOffset;
	float			m_Scale;
	float			m_Scale_x, m_Scale_y;

	bool			m_NeedUpdateVertexData;
};

#endif
