//-----------------------------------------------------------------------------------
/// System.h 系统功能，包括创建渲染窗体和从脚本载入材质
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#ifndef _SYSTEM_H
#define _SYSTEM_H

#include "Singleton.h"
#include "Platform.h"

#include <map>

using namespace std;

namespace Gen
{
#if defined __PLATFORM_LINUX
	namespace X11	// 防止X11的类型名与FreeType冲突
	{
#include <X11/Xlib.h>
#include <X11/extensions/xf86vmode.h>
#include <X11/keysym.h>
#include <X11/Xutil.h>
	}
#endif	// #if defined __PLATFORM_LINUX

	typedef bool(*FileLoadFuncPtr)(const String&, const String&);

	class System : public Singleton<System>
	{
	public:
		System();

		// 从配置文件中载入所需资源
		void LoadResources(const String& filename);

		// 创建渲染窗口
		bool CreateRenderWindow(const String& title,
			unsigned int width,
			unsigned int height,
			unsigned int bits,
			bool fullscreen);

		void DestroyRenderWindow();

		// 设定窗体标题
		//  注：过于频繁的调用会影响fps，不要在update中每帧都调用
		void SetWindowTitle(const String& title);
		void ResizeWindow(unsigned int width, unsigned int height);

		// 鼠标光标控制
		void ToggleMouseCursor(bool toggle);
		void SetMouseCursorPos(int x, int y);
		void CenterMouseCursor();

		// this hacks : 指定窗体句柄，将渲染窗口嵌入到已有窗体中
		void SetRenderWindowHandle(RenderWindowHandle handle)
		{
			m_RenderWindowParam.handle = handle;

#if defined __PLATFORM_WIN32
			RECT rect;
			GetWindowRect(handle, &rect);

			m_RenderWindowParam.height = rect.bottom - rect.top;
			m_RenderWindowParam.width = rect.right - rect.left;
#elif defined __PLATFORM_LINUX
			// TODO: Linux方法再说
#endif	// #if defined __PLATFORM_WIN32

			// TODO: 桌面颜色深度需要取得
			m_RenderWindowParam.colorDepthBit = 32;
		}

		//RenderWindowHandle GetRenderWindowHandle() { return m_RenderWindowParam.handle; }
		RenderWindowParam* GetRenderWindowParameters() { return &m_RenderWindowParam; }

		// 注册指定扩展名的读取函数
		void RegisterExtensionLoader(const String& ext, FileLoadFuncPtr func);

	private:
		RenderWindowParam	m_RenderWindowParam;

		bool				m_FullScreen;
		String				m_TitleName;

		typedef map<const String, FileLoadFuncPtr>	FileLoaders;
		FileLoaders			m_FileLoaders;
	};
}

#endif

