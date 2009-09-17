#ifndef _IOBJECT_H
#define _IOBJECT_H

#include "EString.h"

class IObject
{
public:
	virtual ~IObject() {}

	// ��ʽ���õ����ٶ��󷽷�
	virtual void Destroy() = 0;

	virtual void Update(unsigned long deltaTime) = 0;

	// ����������������Ƿ��Ѿ������¹�
	virtual bool IsUpdated() = 0;

	virtual const String GetTypeName() const = 0;
};

#endif
