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

class System : public Singleton<System>
{
public:
	System();

	void LoadResources(const String& filename);

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

	void ToggleMouseCursor(bool toggle);
	void SetMouseCursorPos(int x, int y);
	void CenterMouseCursor();

	// this hacks : 指定窗体句柄，将渲染窗口嵌入到已有窗体中
	void SetRenderWindowHandle(RenderWindowHandle handle)
	{
		m_RenderWindowParam.handle = handle;

		RECT rect;
		GetWindowRect(handle, &rect);

		m_RenderWindowParam.height = rect.bottom - rect.top;
		m_RenderWindowParam.width = rect.right - rect.left;
		m_RenderWindowParam.color_depth_bit = 32;
	}

	//RenderWindowHandle GetRenderWindowHandle() { return m_RenderWindowParam.handle; }
	RenderWindowParam* GetRenderWindowParameters() { return &m_RenderWindowParam; }

private:
	void LoadAudios(ConfigFileKeys* list);
	void LoadTextures(ConfigFileKeys* list);
	void LoadMaterials(ConfigFileKeys* list);
	void LoadFonts(ConfigFileKeys* list);
	void LoadMeshes(ConfigFileKeys* list);

private:
	RenderWindowParam	m_RenderWindowParam;

	bool				m_FullScreen;
	String				m_TitleName;
};

#endif
