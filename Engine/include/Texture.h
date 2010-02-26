//-----------------------------------------------------------------------------------
/// Texture.h �����࣬�ṩ��������Ĺ�������
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 - 2010 by Mwolf
//-----------------------------------------------------------------------------------
#ifndef _ITEXTURE_H
#define _ITEXTURE_H

#include "Matrix4.h"
#include "EString.h"
#include "IGpuProgram.h"
#include "Resource.h"
#include "IDeviceTexture.h"

using namespace std;

namespace Gen
{
	class TextureManager;

	// ������ģʽ
	// ���������Ե�Ժ�Ĳ�����ʽ
	enum TextureWrapType
	{
		WRAP_TYPE_CLAMP,
		WRAP_TYPE_REPEAT,
		WRAP_TYPE_CLAMP_TO_EDGE,
		WRAP_TYPE_COUNT,
	};

	// �������ģʽ
	// �����ֵ���˵ķ���
	enum TextureFilterType
	{
		FILTER_TYPE_NEAREST,
		FILTER_TYPE_LINEAR,
		FILTER_TYPE_NEAREST_MIPMAP_NEAREST,
		FILTER_TYPE_LINEAR_MIPMAP_NEAREST,
		FILTER_TYPE_NEAREST_MIPMAP_LINEAR,
		FILTER_TYPE_LINEAR_MIPMAP_LINEAR,
	};

	// ��ɫ�������
	enum BlendFactor
	{
		BLEND_FACTOR_ZERO,
		BLEND_FACTOR_ONE,
		BLEND_FACTOR_SRC_COLOR,
		BLEND_FACTOR_ONE_MINUS_SRC_COLOR,
		BLEND_FACTOR_SRC_ALPHA,
		BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
		BLEND_FACTOR_DST_ALPHA,
		BLEND_FACTOR_ONE_MINUS_DST_ALPHA,
		BLEND_FACTOR_DST_COLOR,
		BLEND_FACTOR_ONE_MINUS_DST_COLOR,
		BLEND_FACTOR_SRC_ALPHA_SATURATE,
	};

	// ��������ģʽ
	enum TextureGenMode
	{
		GEN_MODE_TEXCOORD,
		GEN_MODE_EYE_LINEAR,
		GEN_MODE_SPHERE,
		GEN_MODE_CUBE_MAP,
	};

	enum TextureEnvMode
	{
		ENV_MODE_MODULATE,
		ENV_MODE_REPLACE,
		ENV_MODE_ADD,
	};

	class BaseTexture;

	// ������Ⱦ״̬
	typedef struct TextureRenderStateType
	{
		IDeviceTexture*		texture;			///< ����
		String				textureName;		///< ��������		NOTE: ���������Ϊ�˼�¼ֻ���������ƶ�û�����ݵ����

		TextureWrapType		wrapType;			///< ������ģʽ
		TextureFilterType	minFilterType;		///< ������С����
		TextureFilterType	magFilterType;		///< ����Ŵ����

		TextureGenMode		genMode;			///< �Զ�������������ģʽ

		// Eye coordinate
		//bool				useEyeSpaceTex;
		Matrix4				eyeSpaceMatrix;		///< �ӵ�ռ����

		TextureEnvMode		envMode;

		TextureRenderStateType()
		{
			InitValues();
		}

		TextureRenderStateType(IDeviceTexture* tex)
		{
			InitValues();
			texture = tex;
		}

		// ��ʼ��RenderState�ĳ�ֵ
		void InitValues()
		{
			texture = NULL;
			wrapType = WRAP_TYPE_CLAMP_TO_EDGE;
			minFilterType = FILTER_TYPE_LINEAR_MIPMAP_NEAREST;
			magFilterType = FILTER_TYPE_LINEAR;
			genMode = GEN_MODE_TEXCOORD;
			//useEyeSpaceTex = false;

			envMode = ENV_MODE_MODULATE;
		}

	} TextureRenderState;

	// �����������
	// ��������������Դ������������¼�豸�����ָ��
	class BaseTexture : public Resource
	{
		friend class TextureManager;
	public:
		BaseTexture(const String& filename);

		// ----- Overwrite Resource
		//bool Load();
		void UnloadImpl();

		// ��ȡ�豸����
		IDeviceTexture* GetDeviceTexture() const { return m_DeviceTexture; }

	protected:
		IDeviceTexture*		m_DeviceTexture;		///< �豸����

	};

	class Texture2D : public BaseTexture
	{
	public:
		Texture2D(const String& filename);

		// ----- Texture2D Methods

		// ��������
		void Create(unsigned int width, unsigned int height, unsigned int bpp, unsigned char* data);

		// �޸���������
		void ModifyRectData(int xoffset, int yoffset, int width, int height, void* data);
	protected:
		// ----- Overwrite Resource
		bool LoadImpl();

		// ----- Texture2D Methods

	};

	// �����������λ��
	enum CubeMapSide
	{
		CUBE_MAP_POS_X = 0,
		CUBE_MAP_NEG_X,
		CUBE_MAP_POS_Y,
		CUBE_MAP_NEG_Y,
		CUBE_MAP_POS_Z,
		CUBE_MAP_NEG_Z,
	};

	// ����������
	class CubeTexture : public BaseTexture
	{
		//friend class TextureManager;
	public:
		CubeTexture(const String& filename);

		// ��������
		void Create(unsigned int width, unsigned int height, unsigned int bpp, unsigned char* data, int cubeSide);

	protected:
		// ----- Overwrite Resource
		bool LoadImpl();
	};
}

#endif
