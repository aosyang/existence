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



#include "FreeImage/freeimage.h"

namespace Gen
{
	/// @brief
	///	图像类
	///	@par
	///		图像类可以从文件加载图像数据到内存中，提供方法获取图像的详细信息
	class Image
	{
	public:
		/// 构造函数
		Image();

		/// 析构函数
		~Image();

		/// @brief
		///	从文件载入图像
		/// @param filename
		///		图像的文件名
		/// @returns
		///		读取成功，返回true；失败则返回false
		bool LoadFromFile(const String& filename);

		/// 获取图像的宽度
		inline unsigned int GetWidth() { return m_Width; }

		/// 获取图像的高度
		inline unsigned int GetHeight() { return m_Height; }

		/// 获取图像的颜色深度
		inline unsigned int GetBPP() { return m_BitPerPixel; }

		/// 获取图像数据
		unsigned char* GetData();

	private:
		unsigned int	m_Width,			///< 图像宽度
						m_Height;			///< 图像高度
		unsigned int	m_BitPerPixel;		///< 图像颜色深度
		FIBITMAP*		m_Bitmap;			///< FreeImage位图
	};
}

#endif
