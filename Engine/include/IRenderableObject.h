//-----------------------------------------------------------------------------------
/// IRenderableObject.h ����Ⱦ����ӿ�
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------

#ifndef _IRENDERABLEOBJECT_H
#define _IRENDERABLEOBJECT_H

#include "OBB.h"
#include "AABB.h"
#include "Matrix4.h"
#include "Material.h"

// ��һ����Ⱦ���壬��Material����
class IRenderableObject
{
public:
	virtual ~IRenderableObject() {}

	// ��Ⱦ����
	virtual void RenderSingleObject() = 0;

	// ��ȡ����
	//virtual Material* GetMaterial() = 0;

	// �Ƿ�͸��
	virtual bool IsTransparent() = 0;

	virtual	void SetRenderOrder(unsigned int order) = 0;
};

#endif
