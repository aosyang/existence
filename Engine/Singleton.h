//-----------------------------------------------------------------------------------
/// Singleton.h µ¥¼þ
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#ifndef _SINGLETON_H
#define _SINGLETON_H

template <typename T> class Singleton
{
protected:
	Singleton(){}

public:
	static T& Instance()
	{
		static T singleton;

		return singleton;
	}

};

#endif
