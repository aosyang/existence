#ifndef _IOBJECT_H
#define _IOBJECT_H

#include "EString.h"

namespace Gen
{
	class IObject
	{
	public:
		virtual ~IObject() {}

		// 显式调用的销毁对象方法
		virtual void Destroy() = 0;

		virtual void Update(unsigned long deltaTime) = 0;

		virtual const String GetTypeName() const = 0;
	};
}

#endif
