//-----------------------------------------------------------------------------------
/// Image.h 图像类，用于从文件读取图像资源到内存
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------

#ifndef _IMAGE_H
#define _IMAGE_H

#include "EString.h"

using namespace std;

#include "FreeImage/freeimage.h"

namespace Gen
{
	class Image
	{
	public:
		Image();
		~Image();

		bool LoadFromFile(const String& filename);

		inline unsigned int GetWidth() { return m_Width; }
		inline unsigned int GetHeight() { return m_Height; }
		inline unsigned int GetBPP() { return m_BitPerPixel; }
		unsigned char* GetData();

	private:
		unsigned int	m_Width, m_Height;
		unsigned int	m_BitPerPixel;
		FIBITMAP*		m_Bitmap;
	};
}

#endif
