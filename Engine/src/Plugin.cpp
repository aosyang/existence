#include "Plugin.h"

#if defined __PLATFORM_LINUX
#include <dlfcn.h>

#endif	// #if defined __PLATFORM_LINUX

namespace Gen
{
	Plugin::Plugin()
		: m_Module(0)
	{
	}

	Plugin::~Plugin()
	{
		Unload();
	}

	bool Plugin::LoadModule(const String& filename)
	{
#if defined __PLATFORM_WIN32
		m_Module = LoadLibrary(filename.Data());
#elif defined __PLATFORM_LINUX
		m_Module = dlopen(filename.Data(), RTLD_LAZY);
#endif	// #if defined __PLATFORM_WIN32

		if (m_Module)
			return true;

		return false;
	}

	IPlugin* Plugin::CreatePluginInstance()
	{
		if (!m_Module) return NULL;

		CreatePluginFunc func = NULL;
#if defined __PLATFORM_WIN32
		func = (CreatePluginFunc)GetProcAddress(m_Module, "CreatePluginInstance");
#elif defined __PLATFORM_LINUX
		func = (CreatePluginFunc)dlsym(m_Module, "CreatePluginInstance");
#endif	// #if defined __PLATFORM_WIN32

		if (func)
			return (*func)();

		return NULL;
	}

//
//	void* Plugin::GetFunction(const String& name)
//	{
//		if (!m_Module) return NULL;
//
//		void* func = NULL;
//#if defined __PLATFORM_WIN32
//		func = GetProcAddress(m_Module, name.Data());
//#elif defined __PLATFORM_LINUX
//		func = dlsym(m_Module, name.Data());
//#endif	// #if defined __PLATFORM_WIN32
//
//		return func;
//	}

	void Plugin::Unload()
	{
		if (m_Module)
		{
#if defined __PLATFORM_WIN32
			FreeLibrary(m_Module);
#elif defined __PLATFORM_LINUX
			dlclose(m_Module);
#endif	// #if defined __PLATFORM_WIN32
		}
	}

}
