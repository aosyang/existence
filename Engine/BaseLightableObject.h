//-----------------------------------------------------------------------------------
/// BaseLightableObject.h �������������
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------

#ifndef _BASELIGHTABLEOBJECT_H
#define _BASELIGHTABLEOBJECT_H

#include "BaseSceneObject.h"
#include "Light.h"

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
class BaseLightableObject : public BaseSceneObject
{
public:
	BaseLightableObject();

	void AddLight(Light* light);
	void ClearLights();

	void SetMoved(bool moved) { m_Moved = moved; }
	bool HasMoved() const { return m_Moved; }

protected:
	bool		m_Moved;		///< �����Ƿ��ƶ���
	VecLights	m_Lights;		///< Ӱ�����Ĺ�Դ�б�
};

#endif
