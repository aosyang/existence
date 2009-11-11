//-----------------------------------------------------------------------------------
/// System.h ϵͳ���ܣ�����������Ⱦ����ʹӽű��������
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
	namespace X11	// ��ֹX11����������FreeType��ͻ
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

		// �������ļ�������������Դ
		void LoadResources(const String& filename);

		// ������Ⱦ����
		bool CreateRenderWindow(const String& title,
			unsigned int width,
			unsigned int height,
			unsigned int bits,
			bool fullscreen);

		void DestroyRenderWindow();

		// �趨�������
		//  ע������Ƶ���ĵ��û�Ӱ��fps����Ҫ��update��ÿ֡������
		void SetWindowTitle(const String& title);
		void ResizeWindow(unsigned int width, unsigned int height);

		// ��������
		void ToggleMouseCursor(bool toggle);
		void SetMouseCursorPos(int x, int y);
		void CenterMouseCursor();

		// this hacks : ָ��������������Ⱦ����Ƕ�뵽���д�����
		void SetRenderWindowHandle(RenderWindowHandle handle)
		{
			m_RenderWindowParam.handle = handle;

#if defined __PLATFORM_WIN32
			RECT rect;
			GetWindowRect(handle, &rect);

			m_RenderWindowParam.height = rect.bottom - rect.top;
			m_RenderWindowParam.width = rect.right - rect.left;
#elif defined __PLATFORM_LINUX
			// TODO: Linux������˵
#endif	// #if defined __PLATFORM_WIN32

			// TODO: ������ɫ�����Ҫȡ��
			m_RenderWindowParam.colorDepthBit = 32;
		}

		//RenderWindowHandle GetRenderWindowHandle() { return m_RenderWindowParam.handle; }
		RenderWindowParam* GetRenderWindowParameters() { return &m_RenderWindowParam; }

		// ע��ָ����չ���Ķ�ȡ����
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

