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

	void Update(unsigned long deltaTime);
	void Render();
	//void DebugRender();

	// ----- Overwrite BaseSceneObject

	//int GetCollisionType() const { return COLLISION_TYPE_BILLBOARD; }
	// ----- Billboard Methods

	// 材质
	inline Material* GetMaterial() const { return m_Material; }
	inline void SetMaterial(Material* mat) { m_Material = mat; }

	void SetColor(const Color4f& color);

	// 指定公告板的Z方向旋转量(视点方向看去的旋转)
	void SetZRotation(float rad_angle);

	// 指定Z方向与原点的偏移(防止中心被其他物体挡住)
	// TODO FIXME: 偏移会随视点方向变换而移动
	void SetZOffset(float offset);

	// 指定公告板的尺寸
	void SetScale(float scale);

	// 指定公告板屏幕空间缩放
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
