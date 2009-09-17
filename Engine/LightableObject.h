//-----------------------------------------------------------------------------------
/// LightableObject.h �������������
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------

#ifndef _LIGHTABLEOBJECT_H
#define _LIGHTABLEOBJECT_H

#include "Light.h"
#include "RenderableObjectBase.h"

#include <vector>

using namespace std;

typedef vector<Light*>		VecLights;

//-----------------------------------------------------------------------------------
/// \brief
/// �������������
/// 
/// ��¼�˿��Ա������������ܵ�����Щ��Դ��Ӱ��
/// 
/// \remarks
/// �κ��ܹ��ܵ��ƹ�Ӱ��Ķ���Ӧ�ü̳��Դ���
//-----------------------------------------------------------------------------------
class LightableObject
{
public:
	LightableObject();

	void AddLight(Light* light);
	void ClearLights();

	void SetupLights();

	void SetMoved(bool moved) { m_Moved = moved; }
	bool HasMoved() const { return m_Moved; }

	void SetRenderableObject(RenderableObjectBase* rendeable) { m_RenderableObject = rendeable; }
	RenderableObjectBase* GetRenderableObject() { return m_RenderableObject; }

protected:
	bool		m_Moved;		///< �����Ƿ��ƶ���
	VecLights	m_Lights;		///< Ӱ�����Ĺ�Դ�б�

	RenderableObjectBase*	m_RenderableObject;	///< ������������
};

#endif
