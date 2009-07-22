//-----------------------------------------------------------------------------------
/// Font.h �������
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
	ITexture*	texture;			///< �ַ�����������
	float		u1, v1, u2, v2;		///< ��������

	// TODO: remove this, for test only
	wchar_t		wchar;

	unsigned int char_height;		///< ����߶�

	// TODO: ͳһ��λ
	int			width, height;		///< ������
	int			left, top;
	int			advance_x;			///< x������
	//int			advance_y;			// ���û��
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

	// ��ȡһ�������ļ�
	bool LoadFont(const String& font_name, const String& filename);

	// ��ָ�������л��һ���ַ�
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
