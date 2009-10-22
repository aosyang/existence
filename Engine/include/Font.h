//-----------------------------------------------------------------------------------
/// Font.h 字体管理
///
/// File Encoding : GB2312
///
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#ifndef _FONT_H
#define _FONT_H

#include "Singleton.h"
#include "ITexture.h"

#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>

class FontManager;
class Font;

#include "EString.h"
#include <map>

using namespace std;

struct CharacterInfo
{
	ITexture*	texture;			///< 字符所处的纹理
	float		u1, v1, u2, v2;		///< 纹理坐标

	// TODO: remove this, for test only
	wchar_t		wchar;

	unsigned int char_height;		///< 字体高度

	// TODO: 统一单位
	int			width, height;		///< 纹理宽高
	int			left, top;
	int			advance_x;			///< x方向间距
	//int			advance_y;			// 这个没用
};

typedef map<const String, CharacterInfo>	CharacterMap;
typedef map<const String, Font*>			FontMap;

class FontManager : public Singleton<FontManager>
{
	friend class Singleton<FontManager>;
public:
	void Initialize();
	void Shutdown();

	void UnloadAllFonts();

	// 读取一个字体文件
	bool LoadFont(const String& font_name, const String& filename);

	// 从指定字体中获得一个字符
	bool GetCharacter(const String& font_name, const wchar_t wch, unsigned int char_height, CharacterInfo* info);

private:
	FontManager();

	FT_Library		m_Library;

	FontMap			m_FontMap;
	CharacterMap	m_CharacterMap;
};

class Font
{
	friend class FontManager;
public:
	~Font();

private:
	FT_Face m_Face;
};

#endif
