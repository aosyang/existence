//-----------------------------------------------------------------------------------
/// UIRootObject.h UI�����󣬳�������UI���������������
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#ifndef _UIROOTOBJECT_H
#define _UIROOTOBJECT_H

#include "ISceneObject.h"
#include "BaseUIObject.h"

#include <set>

using namespace std;

// TODO FIXME: �ⷽ���޷���֤�ֶ�����SetZOrder�Ŀؼ���ȷ����
class UIObjectComparer : public greater<BaseUIObject*>
{
public:
	bool operator() (BaseUIObject* lhs,BaseUIObject* rhs) const
	{
		//if (lhs->GetZOrder() == rhs->GetZOrder())
		//{
		//	return lhs < rhs;
		//}
		//return lhs->GetZOrder() < rhs->GetZOrder();

		return (lhs->GetZOrder() == rhs->GetZOrder()) ? (lhs < rhs) : (lhs->GetZOrder() < rhs->GetZOrder());

	}
};

typedef set<BaseUIObject*, UIObjectComparer> ChildrenUIObjectsSet;

class UIRootObject : public ISceneObject
{
public:
	~UIRootObject() {}

	// ----- Overwrite ISceneObject

	void Update(unsigned long deltaTime);
	void Render();
	void DebugRender() {}

	// ----- UIRootObject Methods

	void AttachChildObject(BaseUIObject* object);
private:

	ChildrenUIObjectsSet	m_ChildrenObjects;
};

#endif
