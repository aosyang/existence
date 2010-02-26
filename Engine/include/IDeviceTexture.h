//-----------------------------------------------------------------------------------
/// IDeviceTexture.h 设备纹理接口，一个给渲染设备使用的纹理接口
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 - 2010 by Mwolf
//-----------------------------------------------------------------------------------
#ifndef _IDEVICETEXTURE_H
#define _IDEVICETEXTURE_H

namespace Gen
{
	enum TextureType
	{
		TEXTURE_TYPE_2D,
		TEXTURE_TYPE_CUBE,
	};

	// 设备纹理接口，提供渲染器使用的纹理的公共接口
	// 设备纹理是渲染器用于设置纹理状态的对象
	class IDeviceTexture
	{
	public:
		virtual ~IDeviceTexture() {}

		// 通知渲染设备使用这个纹理
		virtual void BindTexture() = 0;

		// 获得纹理的类型(2d或者立方体贴图)
		virtual TextureType GetType() const = 0;
	};

	// 2D材质接口
	class DeviceTexture2D : public IDeviceTexture
	{
	public:
		// 创建纹理
		virtual void Create(unsigned int width, unsigned int height, unsigned int bpp, unsigned char* data) = 0;

		// 修改纹理
		virtual void ModifyRectData(int xoffset, int yoffset, int width, int heigh, void* data) = 0;

		TextureType GetType() const { return TEXTURE_TYPE_2D; }

		unsigned int GetWidth() const { return m_Width; }
		unsigned int GetHeight() const { return m_Height; }
		unsigned int GetBpp() const { return m_Bpp; }
	protected:
		unsigned int	m_Width;
		unsigned int	m_Height;
		unsigned int	m_Bpp;
	};

	class DeviceCubeTexture : public IDeviceTexture
	{
	public:
		// TODO: 立方体贴图需要对六个面单独修改
		virtual void Create(unsigned int width, unsigned int height, unsigned int bpp, unsigned char* data, int cubePos) = 0;

		TextureType GetType() const { return TEXTURE_TYPE_CUBE; }
	};
}

#endif
