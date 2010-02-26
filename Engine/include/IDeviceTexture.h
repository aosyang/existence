//-----------------------------------------------------------------------------------
/// IDeviceTexture.h �豸����ӿڣ�һ������Ⱦ�豸ʹ�õ�����ӿ�
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

	// �豸����ӿڣ��ṩ��Ⱦ��ʹ�õ�����Ĺ����ӿ�
	// �豸��������Ⱦ��������������״̬�Ķ���
	class IDeviceTexture
	{
	public:
		virtual ~IDeviceTexture() {}

		// ֪ͨ��Ⱦ�豸ʹ���������
		virtual void BindTexture() = 0;

		// ������������(2d������������ͼ)
		virtual TextureType GetType() const = 0;
	};

	// 2D���ʽӿ�
	class DeviceTexture2D : public IDeviceTexture
	{
	public:
		// ��������
		virtual void Create(unsigned int width, unsigned int height, unsigned int bpp, unsigned char* data) = 0;

		// �޸�����
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
		// TODO: ��������ͼ��Ҫ�������浥���޸�
		virtual void Create(unsigned int width, unsigned int height, unsigned int bpp, unsigned char* data, int cubePos) = 0;

		TextureType GetType() const { return TEXTURE_TYPE_CUBE; }
	};
}

#endif
