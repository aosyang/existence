//-----------------------------------------------------------------------------------
/// TextUIControl.cpp UI文本控件，可以显示文本
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------

#include "TextUIControl.h"
#include "Engine.h"

namespace Gen
{
	TextUIControl::TextUIControl()
		: m_Font("DefaultFont"),
		m_WText(NULL),
		m_TextLength(1),
		m_RenderTextCount(0),
		m_FontSize(16),
		m_WordSpacing(0),
		m_LineSpacing(0),
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
	}

	void TextUIControl::Render()
	{
		if (m_Dirty)
		{
			RefreshCharacters();
			m_Dirty = false;
		}

		Characters::iterator iter;

		int xpos = 0;
		int ypos = 0;

		// 将所有字依次渲染到屏幕上
		// TODO: 目前每次渲染一个，效率有待提高

		//unsigned int height = 0;
		//for (iter=m_Characters.begin(); iter!=m_Characters.end(); iter++)
		//{
		//	if (iter->charHeight > height) height = iter->charHeight;
		//}

		int width = 0;

		unsigned int count = 0;

		for (iter=m_Characters.begin(); (iter!=m_Characters.end()) && (count<=m_RenderTextCount); iter++)
		{
			count++;
			if (iter->wchar == '\n')
			{
				xpos = 0;

				// hack: 1.2f作为一个调整比例的系数，并不是总是按照比例缩放
				ypos += int(iter->charHeight * 1.2f + m_LineSpacing);

				continue;
			}

			// 宽度超出右边界
			if (width + iter->advance_x>m_Width)
			{
				xpos = 0;
				ypos += int(iter->charHeight * 1.2f + m_LineSpacing);
			}

			int x1 = m_Left + xpos + iter->left;
			int y1 = m_Top + iter->charHeight + 2 - iter->top + ypos;
			int x2 = x1 + iter->width;
			int y2 = y1 + iter->height;
			renderer->RenderScreenQuad(x1, y1, x2, y2, iter->texture, m_TextColor);
			xpos += iter->advance_x + m_WordSpacing;

			width = xpos;
		}

		count = count;
	}

	void TextUIControl::SetFont(const String& fontName)
	{
		m_Font = fontName;

		m_Dirty = true;
	}

	void TextUIControl::SetText(const String& text)
	{

		// 先将string转为wchat_t类型
#if defined __PLATFORM_WIN32
		m_TextLength = MultiByteToWideChar(CP_ACP, 0, text.Data(), -1, NULL, 0);
#elif defined __PLATFORM_LINUX
		m_TextLength = mbstowcs(NULL, text.Data(), 0);
#endif

		SAFE_DELETE_ARRAY(m_WText);
		m_WText = new wchar_t[m_TextLength];

#if defined __PLATFORM_WIN32
		MultiByteToWideChar(CP_ACP, 0, text.Data(), -1, m_WText, m_TextLength);
#elif defined __PLATFORM_LINUX
		mbstowcs(m_WText, text.Data(), m_TextLength);
#endif

		m_RenderTextCount = m_TextLength - 1;

		m_Dirty = true;
	}

	void TextUIControl::RefreshCharacters()
	{
		// 清除原来的文本内容
		m_Characters.clear();

		for (unsigned int i=0; i<m_TextLength; i++)
		{
			CharacterInfo info;

			// 如果任何原因无法获取字符，跳过
			if (!FontManager::Instance().GetCharacter(m_Font, m_WText[i], m_FontSize, &info))
				continue;

			m_Characters.push_back(info);
		}
	}
}
