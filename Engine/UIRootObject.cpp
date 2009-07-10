//-----------------------------------------------------------------------------------
/// UIRootObject.cpp UI根对象，场景所有UI挂载在这个对象上
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
	// 检查重复添加
	if (m_ChildrenObjects.find(object) != m_ChildrenObjects.end())
	{
		return;
	}

	m_ChildrenObjects.insert(object);
}
