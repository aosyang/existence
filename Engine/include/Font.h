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
#include "Texture.h"
#include "ResourceManager.h"
#include "Resource.h"
#include "IDeviceTexture.h"

#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>

#include "EString.h"
#include <map>

using namespace std;

namespace Gen
{
	class FontManager;
	class Font;

	struct CharGlyphInfo
	{
		float	u1, v1, u2, v2;

		wchar_t	wchar;
		int		width, height;

		// Note: FreeType的BearingX与BearingY相当于这里的left与top
		int		left, top;
		int		advance_x;
	};

	class FontManager : public Singleton<FontManager>, public ResourceManagerBase
	{
		friend class Singleton<FontManager>;
	public:
		void Initialize();
		void Shutdown();

		bool CreateResourceHandles(ResourceFileNameInfo* resName);

		Font* GetByName(const String& resName);

		// 在纹理上指定位置绘制字符
		bool BuildTexture(Font* font, const wchar_t wch, unsigned int pixelSize, unsigned int x, unsigned int y, DeviceTexture2D* tex, CharGlyphInfo* charInfo);
	private:
		FontManager();
		~FontManager();
	};

	class Font : public Resource
	{
		friend class FontManager;
	public:

	protected:
		// ----- Overwrite Resource
		bool LoadImpl();
		void UnloadImpl();

		// ----- Font Methods
		Font(const String& filename);
		~Font();

	private:
		FT_Face m_Face;
	};
}

#endif
