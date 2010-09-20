//-----------------------------------------------------------------------------------
/// Image.h ͼ���࣬���ڴ��ļ���ȡͼ����Դ���ڴ�
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
	///	ͼ����
	///	@par
	///		ͼ������Դ��ļ�����ͼ�����ݵ��ڴ��У��ṩ������ȡͼ�����ϸ��Ϣ
	class Image
	{
	public:
		/// ���캯��
		Image();

		/// ��������
		~Image();

		/// @brief
		///	���ļ�����ͼ��
		/// @param filename
		///		ͼ����ļ���
		/// @returns
		///		��ȡ�ɹ�������true��ʧ���򷵻�false
		bool LoadFromFile(const String& filename);

		/// ��ȡͼ��Ŀ��
		inline unsigned int GetWidth() { return m_Width; }

		/// ��ȡͼ��ĸ߶�
		inline unsigned int GetHeight() { return m_Height; }

		/// ��ȡͼ�����ɫ���
		inline unsigned int GetBPP() { return m_BitPerPixel; }

		/// ��ȡͼ������
		unsigned char* GetData();

	private:
		unsigned int	m_Width,			///< ͼ����
						m_Height;			///< ͼ��߶�
		unsigned int	m_BitPerPixel;		///< ͼ����ɫ���
		FIBITMAP*		m_Bitmap;			///< FreeImageλͼ
	};
}

#endif
