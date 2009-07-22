//-----------------------------------------------------------------------------------
/// TextUIControl.h UI文本控件，可以显示文本
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------

#ifndef _TEXTUICONTROL_H
#define _TEXTUICONTROL_H

#include "BaseUIObject.h"
#include "Platform.h"
#include "Font.h"
#include "Color4f.h"

#include "EString.h"
#include <vector>

using namespace std;

class TextUIControl : public BaseUIObject
{
public:
	TextUIControl();
	~TextUIControl();

	// ----- Overwrite ISceneObject

	void Update(unsigned long deltaTime);
	void Render();

	// ----- TextUIControl Methods

	// 指定文本使用的字体
	void SetFont(const String& font_name);

	// 指定要显示的文本
	void SetText(const String& text);

	unsigned int GetTextLength() const { return m_TextLength; }
	
	void SetRenderTextCount(unsigned int count) { m_RenderTextCount = count; }
	unsigned int GetRenderTextCount() const { return m_RenderTextCount; }

	// 指定文本颜色
	void SetTextColor(const Color4f& color) { m_TextColor = color; }
	Color4f GetTextColor() const { return m_TextColor; }

	// 字体大小
	void SetFontSize(unsigned int size) { m_FontSize = size; }
	unsigned int GetFontSize() const { return m_FontSize; }

	// 字间距
	void SetWordSpacing(int spacing) { m_WordSpacing = spacing; }
	int GetWordSpacing() const { return m_WordSpacing; }

	// 行间距
	void SetLineSpacing(int spacing) { m_LineSpacing = spacing; }
	int GetLineSpacing() const { return m_LineSpacing; }

private:
	void RefreshCharacters();

private:
	String			m_Font;				///< 字体
	Color4f			m_TextColor;		///< 文本颜色
	wchar_t*		m_WText;			///< 文本
	unsigned int	m_TextLength;		///< 文本长度
	unsigned int	m_RenderTextCount;	///< 实际渲染的字数

	unsigned int	m_FontSize;			///< 字体大小

	int				m_WordSpacing;		///< 字间距
	int				m_LineSpacing;		///< 行间距

	bool			m_Dirty;			///< 是否需要更新字符信息

	typedef vector<CharacterInfo> Characters;
	Characters		m_Characters;		///< 每个字的信息
};

#endif
