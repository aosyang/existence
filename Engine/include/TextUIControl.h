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
#include "IVertexBuffer.h"

#include "EString.h"
#include <vector>
#include <set>



namespace Gen
{
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
		void SetFont(const String& fontName);

		// ָ��Ҫ��ʾ���ı�
		void SetText(const String& text);

		unsigned int GetTextLength() const { return m_TextLength; }

		// ָ���ı���ɫ
		void SetTextColor(const Color4f& color) { m_TextColor = color; }
		Color4f GetTextColor() const { return m_TextColor; }

		// �����С
		void SetFontSize(float size) { m_FontSize = size; }
		float GetFontSize() const { return m_FontSize; }

		// �ּ��
		void SetWordSpacing(int spacing) { m_WordSpacing = spacing; }
		int GetWordSpacing() const { return m_WordSpacing; }

		// �м��
		void SetLineSpacing(int spacing) { m_LineSpacing = spacing; }
		int GetLineSpacing() const { return m_LineSpacing; }

	private:
		void RefreshCharacters();

		void BuildVertexBuffer();

		// ������ģ����
		void EnlargeGlyphTexture();

	private:
		String				m_FontName;				///< ������
		Font*				m_Font;					///< ����
		Color4f				m_TextColor;			///< �ı���ɫ
		wchar_t*			m_WText;				///< �ı�
		unsigned int		m_TextLength;			///< �ı�����

		float				m_FontSize;				///< �����С

		int					m_WordSpacing;			///< �ּ��
		int					m_LineSpacing;			///< �м��

		bool				m_Dirty;				///< �Ƿ���Ҫ�����ַ���Ϣ

		typedef std::vector<CharGlyphInfo> Characters;
		Characters			m_Characters;			///< ÿ���ֵ���Ϣ

		DeviceTexture2D*	m_GlyphTexture;
		std::set<wchar_t>		m_WideCharacters;		///< �ַ����ϣ�����ͳ���ı���ǰʹ�õ��ַ�����
		int					m_CharacterPixelSize;	///< �ַ��ߴ�
		unsigned int		m_GlyphTexSize;			///< ��ģ����ߴ�
		int					m_VisibleCharCount;		///< �ɼ��ַ���Ŀ

		typedef std::map<wchar_t, CharGlyphInfo>		CharacterGlyphInfo;
		CharacterGlyphInfo	m_CharGlyphInfo;

		IVertexBuffer*		m_VertexBuffer;
		IIndexBuffer*		m_IndexBuffer;
	};
}

#endif
