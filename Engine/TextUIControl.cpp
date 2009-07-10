//-----------------------------------------------------------------------------------
/// TextUIControl.cpp UI文本控件，可以显示文本
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------

#include "TextUIControl.h"
#include "Engine.h"

TextUIControl::TextUIControl()
: m_Font("DefaultFont"),
  m_WText(NULL),
  m_TextSize(1),
  m_Dirty(true),
  BaseUIObject()
{
	m_WText = new wchar_t[1];
	m_WText[0] = 0;

}

TextUIControl::~TextUIControl()
{
	SAFE_DELETE_ARRAY(m_WText);
}

void TextUIControl::Update(unsigned long deltaTime)
{
	if (m_Dirty)
	{
		RefreshCharacters();
		m_Dirty = false;
	}
}

void TextUIControl::Render()
{
	Characters::iterator iter;

	int xpos = 0;
	int ypos = 0;

	// 将所有字依次渲染到屏幕上
	// TODO: 目前每次渲染一个，效率有待提高

	//unsigned int height = 0;
	//for (iter=m_Characters.begin(); iter!=m_Characters.end(); iter++)
	//{
	//	if (iter->char_height > height) height = iter->char_height;
	//}

	for (iter=m_Characters.begin(); iter!=m_Characters.end(); iter++)
	{
		if (iter->wchar == '\n')
		{
			xpos = 0;
			ypos += iter->char_height;

			continue;
		}

		int x1 = m_Left + xpos + iter->left;
		int y1 = m_Top + iter->char_height + 2 - iter->top + ypos;
		int x2 = x1 + iter->width;
		int y2 = y1 + iter->height;
		renderer->RenderScreenQuad(x1, y1, x2, y2, iter->texture, m_TextColor);
		xpos += iter->advance_x;
	}
}

void TextUIControl::SetFont(const string& font_name)
{
	m_Font = font_name;

	m_Dirty = true;
}

void TextUIControl::SetText(const string& text)
{

	// 先将string转为wchat_t类型
#if defined __PLATFORM_WIN32
	m_TextSize = MultiByteToWideChar(CP_ACP, 0, text.data(), -1, NULL, 0);
#elif defined __PLATFORM_LINUX
	m_TextSize = mbstowcs(NULL, text.data(), 0);
#endif

	SAFE_DELETE_ARRAY(m_WText);
	m_WText = new wchar_t[m_TextSize];

#if defined __PLATFORM_WIN32
	MultiByteToWideChar(CP_ACP, 0, text.data(), -1, m_WText, m_TextSize);
#elif defined __PLATFORM_LINUX
	mbstowcs(m_WText, text.data(), m_TextSize);
#endif

	m_Dirty = true;
}

void TextUIControl::RefreshCharacters()
{
	// 清除原来的文本内容
	m_Characters.clear();

	for (unsigned int i=0; i<m_TextSize; i++)
	{
		CharacterInfo info;

		// 如果任何原因无法获取字符，跳过
		if (!FontManager::Instance().GetCharacter(m_Font, m_WText[i], 16, &info))
			continue;

		m_Characters.push_back(info);
	}
}

