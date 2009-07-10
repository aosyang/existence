//-----------------------------------------------------------------------------------
/// TextUIControl.h UI�ı��ؼ���������ʾ�ı�
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

	// ָ���ı�ʹ�õ�����
	void SetFont(const string& font_name);

	// ָ��Ҫ��ʾ���ı�
	void SetText(const string& text);

	// ָ���ı���ɫ
	void SetTextColor(const Color4f& color) { m_TextColor = color; }

private:
	void RefreshCharacters();

private:
	string			m_Font;				///< ����
	Color4f			m_TextColor;		///< �ı���ɫ
	wchar_t*		m_WText;			///< �ı�
	unsigned int	m_TextSize;			///< �ı�����

	bool			m_Dirty;			///< �Ƿ���Ҫ�����ַ���Ϣ

	typedef vector<CharacterInfo> Characters;
	Characters		m_Characters;		///< ÿ���ֵ���Ϣ
};

#endif
