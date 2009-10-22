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

	// ָ���ı�ʹ�õ�����
	void SetFont(const String& font_name);

	// ָ��Ҫ��ʾ���ı�
	void SetText(const String& text);

	unsigned int GetTextLength() const { return m_TextLength; }
	
	void SetRenderTextCount(unsigned int count) { m_RenderTextCount = count; }
	unsigned int GetRenderTextCount() const { return m_RenderTextCount; }

	// ָ���ı���ɫ
	void SetTextColor(const Color4f& color) { m_TextColor = color; }
	Color4f GetTextColor() const { return m_TextColor; }

	// �����С
	void SetFontSize(unsigned int size) { m_FontSize = size; }
	unsigned int GetFontSize() const { return m_FontSize; }

	// �ּ��
	void SetWordSpacing(int spacing) { m_WordSpacing = spacing; }
	int GetWordSpacing() const { return m_WordSpacing; }

	// �м��
	void SetLineSpacing(int spacing) { m_LineSpacing = spacing; }
	int GetLineSpacing() const { return m_LineSpacing; }

private:
	void RefreshCharacters();

private:
	String			m_Font;				///< ����
	Color4f			m_TextColor;		///< �ı���ɫ
	wchar_t*		m_WText;			///< �ı�
	unsigned int	m_TextLength;		///< �ı�����
	unsigned int	m_RenderTextCount;	///< ʵ����Ⱦ������

	unsigned int	m_FontSize;			///< �����С

	int				m_WordSpacing;		///< �ּ��
	int				m_LineSpacing;		///< �м��

	bool			m_Dirty;			///< �Ƿ���Ҫ�����ַ���Ϣ

	typedef vector<CharacterInfo> Characters;
	Characters		m_Characters;		///< ÿ���ֵ���Ϣ
};

#endif
