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

#include <string>
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
	void SetFont(const string& font_name);

	// 指定要显示的文本
	void SetText(const string& text);

	// 指定文本颜色
	void SetTextColor(const Color4f& color) { m_TextColor = color; }

private:
	void RefreshCharacters();

private:
	string			m_Font;				///< 字体
	Color4f			m_TextColor;		///< 文本颜色
	wchar_t*		m_WText;			///< 文本
	unsigned int	m_TextSize;			///< 文本长度

	bool			m_Dirty;			///< 是否需要更新字符信息

	typedef vector<CharacterInfo> Characters;
	Characters		m_Characters;		///< 每个字的信息
};

#endif
