//-----------------------------------------------------------------------------------
/// TextureManager.cpp 材质管理器实现
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 - 2010 by Mwolf
//-----------------------------------------------------------------------------------
#include "TextureManager.h"
#include "Texture.h"
#include "Image.h"
#include "Engine.h"
#include "FileSystem.h"

namespace Gen
{
	bool TextureManager::CreateResourceHandles(ResourceFileNameInfo* resName)
	{
		String relativePathName = resName->path + CORRECT_SLASH + resName->filename;

		String ext = FileSystem::Instance().GetExtension(relativePathName);
		ext.ToLowerCase();

		BaseTexture* texture;

		// 立方体纹理
		if (ext==".ect")
			texture = new CubeTexture(relativePathName);
		else
			texture = new Texture2D(relativePathName);

		AssertFatal(m_ResourceMap.find(resName->filename)==m_ResourceMap.end(),
					"TextureManager::CreateResourceHandles(): Specified resource name already exists.");

		m_ResourceMap[resName->filename] = texture;
		texture->m_ResourceName = resName->filename;
		return true;
	}

	Texture2D* TextureManager::CreateTexture2D(const String& resName)
	{
		String name = CheckResName(resName);

		Texture2D* texture = new Texture2D("");

		m_ResourceMap[name] = texture;
		texture->m_ResourceName = name;
		return texture;
	}


	BaseTexture* TextureManager::GetByName(const String& resName)
	{
		return static_cast<BaseTexture*>(GetResourceByName(resName));
	}
}
