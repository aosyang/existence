//-----------------------------------------------------------------------------------
/// Texture.cpp 纹理类实现
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 - 2010 by Mwolf
//-----------------------------------------------------------------------------------
#include "Texture.h"
#include "Engine.h"
#include "Image.h"
#include "Renderer.h"
#include "FileSystem.h"
#include <fstream>



namespace Gen
{

	BaseTexture::BaseTexture(const String& filename)
		: Resource(filename),
		  m_DeviceTexture(NULL)
	{

	}

	void BaseTexture::UnloadImpl()
	{
		SAFE_DELETE(m_DeviceTexture);
	}

	// ----------------------------------- Texture2D

	Texture2D::Texture2D(const String& filename)
		: BaseTexture(filename)
	{
	}

	bool Texture2D::LoadImpl()
	{
		Image* image = new Image();
		if (!image->LoadFromFile(m_Filename))
		{
			delete image;
			return false;
		}

		this->Create(	image->GetWidth(),
						image->GetHeight(),
						image->GetBPP(),
						image->GetData());

		delete image;

		return true;
	}

	void Texture2D::Create(unsigned int width, unsigned int height, unsigned int bpp, unsigned char* data)
	{
		AssertFatal(!m_IsResourceLoaded, "Texture2D::Create(): Unable to create texture: texture has been created already");
		// 使用渲染器生成一个纹理
		m_DeviceTexture = Renderer::Instance().BuildTexture2D();

		static_cast<DeviceTexture2D*>(m_DeviceTexture)->Create(width, height, bpp, data);

		m_IsResourceLoaded = true;
	}

	void Texture2D::ModifyRectData(int xoffset, int yoffset, int width, int height, void* data)
	{
		static_cast<DeviceTexture2D*>(m_DeviceTexture)->ModifyRectData(xoffset, yoffset, width, height, data);
	}

	// ----------------------------------- CubeTexture

	CubeTexture::CubeTexture(const String& filename)
		: BaseTexture(filename)
	{

	}
	
	void CubeTexture::Create(unsigned int width, unsigned int height, unsigned int bpp, unsigned char* data, int cubeSide)
	{
		AssertFatal(!m_IsResourceLoaded, "CubeTexture::Create(): Unable to create texture: texture has been created already");

		static_cast<DeviceCubeTexture*>(m_DeviceTexture)->Create(width, height, bpp, data, cubeSide);
	}

	bool CubeTexture::LoadImpl()
	{
		// TODO List
		// 读取.ect立方体材质文件
		// 去掉注释

		std::ifstream fin(m_Filename.Data());

		if (!fin.is_open()) return false;

		char texName[6][256];
		int index = 0;

		// 材质名
		for (int i=0; i<6; i++)
		{
			fin.getline(texName[i], 256);
			if (fin.eof() && i!=5) return false;
		}

		m_DeviceTexture = Renderer::Instance().BuildCubeTexture();

		String path;
		size_t pos = m_Filename.FindLastOf(CORRECT_SLASH);
		if (pos!=String::npos)
		{
			path = m_Filename.Substr(0, pos+1);
		}

		for (int i=0; i<6; i++)
		{
			Image* image = new Image();

			String filename = path + String(texName[i]);

			// TODO: 如果某个文件读取失败，置换为默认纹理
			image->LoadFromFile(filename);

			static_cast<DeviceCubeTexture*>(m_DeviceTexture)->Create(	image->GetWidth(),
																		image->GetHeight(),
																		image->GetBPP(),
																		image->GetData(),
																		i);

			delete image;
		}

		return true;
	}
}
