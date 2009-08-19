#ifndef _PLUGIN_H
#define _PLUGIN_H

#if defined __PLATFORM_WIN32
typedef HINSTANCE Module_t;
#define LoadModule(filename) LoadLibrary(filename)
#define GetFunction(module, name) GetProcAddress(module, name)
#define UnloadModule(module) FreeLibrary(module)
#elif defined __PLATFORM_LINUX
#include <dlfcn.h>
typedef void* Module_t;
#define LoadModule(filename) dlopen(filename, RTLD_LAZY)
#define GetFunction(module, name) dlsym(module, name)
#define UnloadModule(module) dlclose(module)
#endif	// #if defined __PLATFORM_WIN32

#endif
