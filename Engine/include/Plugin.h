#ifndef _PLUGIN_H
#define _PLUGIN_H

#include "Platform.h"
#include "EString.h"
#include "IPlugin.h"

namespace Gen
{

#if defined __PLATFORM_WIN32
	typedef HINSTANCE Module_t;

#elif defined __PLATFORM_LINUX
	typedef void* Module_t;

#endif	// #if defined __PLATFORM_WIN32

	/// @brief
	/// 引擎插件
	/// @par
	///		通过接口提供无须编译到引擎代码的外部功能
	class Plugin
	{
	public:
		Plugin();
		~Plugin();

		bool LoadModule(const String& filename);

		/// @brief
		/// 创建插件实例
		/// @remarks
		///		这个方法必须确保目标对象中具有CreatePluginInstance这一方法
		IPlugin* CreatePluginInstance();
		//void* GetFunction(const String& name);

		void Unload();

	private:
		Module_t		m_Module;		///< 插件句柄
	};

}

#endif
