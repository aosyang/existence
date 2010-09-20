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

		// 指定文本使用的字体
		void SetFont(const String& fontName);

		// 指定要显示的文本
		void SetText(const String& text);

		unsigned int GetTextLength() const { return m_TextLength; }

		// 指定文本颜色
		void SetTextColor(const Color4f& color) { m_TextColor = color; }
		Color4f GetTextColor() const { return m_TextColor; }

		// 字体大小
		void SetFontSize(float size) { m_FontSize = size; }
		float GetFontSize() const { return m_FontSize; }

		// 字间距
		void SetWordSpacing(int spacing) { m_WordSpacing = spacing; }
		int GetWordSpacing() const { return m_WordSpacing; }

		// 行间距
		void SetLineSpacing(int spacing) { m_LineSpacing = spacing; }
		int GetLineSpacing() const { return m_LineSpacing; }

	private:
		void RefreshCharacters();

		void BuildVertexBuffer();

		// 扩大字模纹理
		void EnlargeGlyphTexture();

	private:
		String				m_FontName;				///< 字体名
		Font*				m_Font;					///< 字体
		Color4f				m_TextColor;			///< 文本颜色
		wchar_t*			m_WText;				///< 文本
		unsigned int		m_TextLength;			///< 文本长度

		float				m_FontSize;				///< 字体大小

		int					m_WordSpacing;			///< 字间距
		int					m_LineSpacing;			///< 行间距

		bool				m_Dirty;				///< 是否需要更新字符信息

		typedef std::vector<CharGlyphInfo> Characters;
		Characters			m_Characters;			///< 每个字的信息

		DeviceTexture2D*	m_GlyphTexture;
		std::set<wchar_t>		m_WideCharacters;		///< 字符集合，用于统计文本框当前使用的字符数量
		int					m_CharacterPixelSize;	///< 字符尺寸
		unsigned int		m_GlyphTexSize;			///< 字模纹理尺寸
		int					m_VisibleCharCount;		///< 可见字符数目

		typedef std::map<wchar_t, CharGlyphInfo>		CharacterGlyphInfo;
		CharacterGlyphInfo	m_CharGlyphInfo;

		IVertexBuffer*		m_VertexBuffer;
		IIndexBuffer*		m_IndexBuffer;
	};
}

#endif
