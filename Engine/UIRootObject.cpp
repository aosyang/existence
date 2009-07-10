//-----------------------------------------------------------------------------------
/// UIRootObject.cpp UI�����󣬳�������UI���������������
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------

#include "UIRootObject.h"


void UIRootObject::Update(unsigned long deltaTime)
{
	ChildrenUIObjectsSet::iterator iter;
	for (iter = m_ChildrenObjects.begin();
		iter != m_ChildrenObjects.end();
		iter++)
	{
		(*iter)->Update(deltaTime);
	}
}

void UIRootObject::Render()
{
	ChildrenUIObjectsSet::iterator iter;
	for (iter = m_ChildrenObjects.begin();
		iter != m_ChildrenObjects.end();
		iter++)
	{
		(*iter)->Render();
	}
}

void UIRootObject::AttachChildObject(BaseUIObject *object)
{
	// ����ظ����
	if (m_ChildrenObjects.find(object) != m_ChildrenObjects.end())
	{
		return;
	}

	m_ChildrenObjects.insert(object);
}
