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
#include "TextureManager.h"
#include "FileSystem.h"

namespace Gen
{
	static FT_Library	g_FTLibrary;

	FontManager::FontManager()
	{
	}

	FontManager::~FontManager()
	{

	}

	void FontManager::Initialize()
	{
		Log.MsgLn("Initializing font manager");
		// 初始化FreeType
		AssertFatal(!FT_Init_FreeType(&g_FTLibrary), "FontManager::Initialize() : Failed to initialize freetype.");
	}

	void FontManager::Shutdown()
	{
		Log.MsgLn("Shutting down font manager");
		FT_Done_FreeType(g_FTLibrary);
	}
	
	bool FontManager::CreateResourceHandles(ResourceFileNameInfo* resName)
	{
		String relativePathName = resName->path + CORRECT_SLASH + resName->filename;

		// 获取字体信息
		FT_Face ft_face;
		FT_New_Face(g_FTLibrary, relativePathName.Data(), 0, &ft_face);

		Font* font = new Font(relativePathName);

		AssertFatal(m_ResourceMap.find(ft_face->family_name)==m_ResourceMap.end(),
					"FontManager::CreateResourceHandles(): Specified resource name already exists.");
		m_ResourceMap[ft_face->family_name] = font;
		font->m_ResourceName = resName->filename;

		FT_Done_Face(ft_face);

		return true;
	}
	
	Font* FontManager::GetByName(const String& resName)
	{
		return static_cast<Font*>(GetResourceByName(resName));
	}

	bool FontManager::BuildTexture(Font* font, const wchar_t wch, unsigned int pixelSize, unsigned int x, unsigned int y, DeviceTexture2D* tex, CharGlyphInfo* charInfo)
	{
		if (!font) return false;

		// 获取该字符的象素信息，写入纹理
		FT_GlyphSlot slot;

		// 设置象素尺寸
		FT_Set_Pixel_Sizes(font->m_Face, pixelSize, 0);

		if (FT_Load_Char(font->m_Face, wch, FT_LOAD_RENDER))
		{
			return false;
		}

		slot = font->m_Face->glyph;

		FT_Bitmap& bitmap = slot->bitmap;

		charInfo->wchar = wch;
		charInfo->advance_x = slot->advance.x >> 6;
		charInfo->height = bitmap.rows;
		charInfo->width = bitmap.width;
		charInfo->left = slot->bitmap_left;
		charInfo->top = slot->bitmap_top;

		if (bitmap.width && bitmap.rows)
		{
			unsigned char* buf = new unsigned char[pixelSize * pixelSize * 4];
			for(int j=0; j  < pixelSize ; j++)
			{
				for(int i=0; i < pixelSize; i++)
				{
					int index = 4 * i + (pixelSize - j - 1) * pixelSize * 4;
					unsigned char alpha_val =  (i>=bitmap.width || j>=bitmap.rows) ? 0 : bitmap.buffer[i + bitmap.width*j];

					// 白色字体，使用Alpha通道描绘字形
					buf[index] = 0xff;
					buf[index + 1] = 0xff;
					buf[index + 2] = 0xff;
					buf[index + 3] = alpha_val;
				}
			}

			tex->ModifyRectData(x, y, pixelSize, pixelSize, buf);

			delete [] buf;

			return true;
		}

		// 没有这个字符的数据，不绘制到纹理，但保留字符的前进信息
		return false;
	}

	// ----------------------------------- Font

	Font::Font(const String& filename)
		: Resource(filename)
	{
	}

	Font::~Font()
	{
	}

	bool Font::LoadImpl()
	{
		if (FT_New_Face(g_FTLibrary, m_Filename.Data(), 0, &m_Face))
		{
			//cout << "FT_New_Face failed (there is probably a problem with your font file)" << endl;
			return false;
		}

		return true;
	}
	
	void Font::UnloadImpl()
	{
		FT_Done_Face(m_Face);
	}

}
