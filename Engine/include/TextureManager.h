//-----------------------------------------------------------------------------------
/// TextureManager.h ���ʹ�������������������ֲ���
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

		// ��ȡָ�����Ƶ�����
		BaseTexture* GetByName(const String& resName);
	};

}

#endif
