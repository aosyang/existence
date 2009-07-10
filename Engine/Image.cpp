//-----------------------------------------------------------------------------------
/// Image.cpp ͼ�����ʵ��
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------

#include "Image.h"

Image::Image()
 : m_Width(0), m_Height(0),
   m_BitPerPixel(0),
   m_Bitmap(0)
{
}

Image::~Image()
{
	FreeImage_Unload(m_Bitmap);
}

bool Image::LoadFromFile(const string& filename)
{
	// ͼ���ʽ
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	// ͼ������ָ��
	BYTE* bits(0);

	// ��ȡͼ���ʽ
	fif = FreeImage_GetFileType(filename.data(), 0);
	// ����޷���ȡ�����Դ��ļ���չ��ȷ����ʽ
	if(fif == FIF_UNKNOWN) 
		fif = FreeImage_GetFIFFromFilename(filename.data());
	// ��Ȼ�޷���ȡ������ʧ��
	if(fif == FIF_UNKNOWN)
		return false;

	//check that the plugin has reading capabilities and load the file
	if(FreeImage_FIFSupportsReading(fif))
		m_Bitmap = FreeImage_Load(fif, filename.data());
	//if the image failed to load, return failure
	if(!m_Bitmap)
		return false;

	m_BitPerPixel = FreeImage_GetBPP(m_Bitmap);
	//m_Bitmap = FreeImage_ConvertTo32Bits(m_Bitmap);

	//retrieve the image data
	bits = FreeImage_GetBits(m_Bitmap);
	//get the image width and height
	m_Width = FreeImage_GetWidth(m_Bitmap);
	m_Height = FreeImage_GetHeight(m_Bitmap);
	//if this somehow one of these failed (they shouldn't), return failure
	if((bits == 0) || (m_Width == 0) || (m_Height == 0))
		return false;
	
	return true;
}

unsigned char* Image::GetData()
{
	if (!m_Bitmap || (m_Width == 0) || (m_Height == 0))
		return 0;

	return FreeImage_GetBits(m_Bitmap);
}
