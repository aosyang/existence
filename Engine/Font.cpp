//-----------------------------------------------------------------------------------
/// Font.cpp 字体管理
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------

#include "Font.h"

#include "Debug.h"
#include "Engine.h"

FontManager::FontManager()
{
}

void FontManager::Initialize()
{
	Log.MsgLn("Initializing font manager");
	// 初始化FreeType
	AssertFatal(!FT_Init_FreeType(&m_Library), "FontManager::Initialize() : FT_Init_FreeType failed.");
}

void FontManager::Shutdown()
{
	Log.MsgLn("Shutting down font manager");
	UnloadAllFonts();
	FT_Done_FreeType(m_Library);
}

void FontManager::UnloadAllFonts()
{
	FontMap::iterator iter;
	for (iter=m_FontMap.begin(); iter!=m_FontMap.end(); iter++)
	{
		delete (*iter).second;
	}
}

bool FontManager::LoadFont(const String& font_name, const String& filename)
{
	if (m_FontMap.find(font_name) != m_FontMap.end())
	{
		// 已经存在的字体名
		return false;
	}

	Font* font = new Font();

	if (FT_New_Face(m_Library, filename.Data(), 0, &font->m_Face))
	{
		//cout << "FT_New_Face failed (there is probably a problem with your font file)" << endl;
		delete font;
		return false;
	}

	m_FontMap[font_name] = font;
	return true;
}

bool FontManager::GetCharacter(const String& font_name, const wchar_t wch, unsigned int char_height, CharacterInfo* info)
{
	if (m_FontMap.find(font_name) == m_FontMap.end())
		return false;

	// hacks: return if wch==0
	if (!wch)
		return false;

	Font* font = m_FontMap[font_name];

	// TODO: 将所有集中字体缓存在几张纹理中

	char name[256];
	sprintf(name, "#Font_%s_%d_d", font_name.Data(), wch, char_height);

	if (m_CharacterMap.find(name) == m_CharacterMap.end())

	//if (!(info->texture = renderer->GetTexture(name)))
	{
		CharacterInfo newInfo;


		//static unsigned int h = 16;
		//static unsigned int h = 32;
		//unsigned int h = CharacterInfo::char_height;

		FT_Set_Char_Size(font->m_Face, char_height << 6, char_height << 6, 96, 96);

		if (FT_Load_Glyph(font->m_Face, FT_Get_Char_Index(font->m_Face, wch), FT_LOAD_DEFAULT))
		{
			return false;
		}

		FT_Glyph glyph;
		if (FT_Get_Glyph(font->m_Face->glyph, &glyph))
		{
			return false;
		}

		FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_NORMAL, 0, 1);
		FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph;

		FT_Bitmap& bitmap = bitmap_glyph->bitmap;

		if (bitmap.width && bitmap.rows)
		{

			int width = bitmap.width;
			int height = bitmap.rows;

			unsigned char* buf = new unsigned char[width * height * 4];
			for(int j=0; j  < height ; j++)
			{
				for(int i=0; i < width; i++)
				{
					int index = 4 * i + (height - j - 1) * width * 4;
					unsigned char alpha_val =  (i>=bitmap.width || j>=bitmap.rows) ? 0 : bitmap.buffer[i + bitmap.width*j];

					// 白色字体，使用Alpha通道描绘字形
					buf[index] = 0xff;
					buf[index + 1] = 0xff;
					buf[index + 2] = 0xff;
					buf[index + 3] = alpha_val;
				}
			}

			// 宽高必须是2的n次幂
			newInfo.texture = renderer->BuildTexture(name, width, height, 32, buf);

			// 使用nearest过滤效果不佳
			//newInfo.texture->SetMagFilterType(FILTER_TYPE_NEAREST);
			//newInfo.texture->SetMinFilterType(FILTER_TYPE_NEAREST);

			delete [] buf;
		}
		else 
			newInfo.texture = NULL;

		newInfo.wchar = wch;
		newInfo.char_height = char_height;
		newInfo.advance_x = font->m_Face->glyph->advance.x >> 6;
		//newInfo.advance_y = font->m_Face->glyph->advance.y >> 6;
		newInfo.height = bitmap.rows;
		newInfo.width = bitmap.width;
		newInfo.left = bitmap_glyph->left;
		newInfo.top = bitmap_glyph->top;
		m_CharacterMap[name] = newInfo;

	}

	*info = m_CharacterMap[name];

	return true;

}

Font::~Font()
{
	FT_Done_Face(m_Face);
}
