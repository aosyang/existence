//-----------------------------------------------------------------------------------
/// TextureManager.h 材质管理器，创建、管理各种材质
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 - 2010 by Mwolf
//-----------------------------------------------------------------------------------
#ifndef _TEXTUREMANAGER_H
#define _TEXTUREMANAGER_H

#include "ResourceManager.h"

namespace Gen
{
	class BaseTexture;
	class Texture2D;

	class TextureManager : public Singleton<TextureManager>, public ResourceManagerBase
	{
		friend class Singleton<TextureManager>;
	public:
		bool CreateResourceHandles(ResourceFileNameInfo* resName);

		Texture2D* CreateTexture2D(const String& resName);

		// 获取指定名称的纹理
		BaseTexture* GetByName(const String& resName);
	};

}

#endif
