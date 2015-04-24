//-----------------------------------------------------------------------------------
/// UIRootObject.h UI根对象，场景所有UI挂载在这个对象上
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
#include <functional>


namespace Gen
{
	// TODO FIXME: 这方法无法保证手动重新SetZOrder的控件正确排序
	class UIObjectComparer : public std::greater<BaseUIObject*>
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

	typedef std::set<BaseUIObject*, UIObjectComparer> ChildrenUIObjectsSet;

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
		void RemoveAllChildObjects();
	private:

		ChildrenUIObjectsSet	m_ChildrenObjects;
	};
}

#endif

