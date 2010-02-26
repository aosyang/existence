//-----------------------------------------------------------------------------------
/// Texture.h 纹理类，提供纹理操作的公共方法
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

	// 纹理环绕模式
	// 超出纹理边缘以后的采样方式
	enum TextureWrapType
	{
		WRAP_TYPE_CLAMP,
		WRAP_TYPE_REPEAT,
		WRAP_TYPE_CLAMP_TO_EDGE,
		WRAP_TYPE_COUNT,
	};

	// 纹理过滤模式
	// 纹理插值过滤的方法
	enum TextureFilterType
	{
		FILTER_TYPE_NEAREST,
		FILTER_TYPE_LINEAR,
		FILTER_TYPE_NEAREST_MIPMAP_NEAREST,
		FILTER_TYPE_LINEAR_MIPMAP_NEAREST,
		FILTER_TYPE_NEAREST_MIPMAP_LINEAR,
		FILTER_TYPE_LINEAR_MIPMAP_LINEAR,
	};

	// 颜色混合因子
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

	// 纹理生成模式
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

	// 纹理渲染状态
	typedef struct TextureRenderStateType
	{
		IDeviceTexture*		texture;			///< 纹理
		String				textureName;		///< 纹理名称		NOTE: 这个变量是为了记录只有纹理名称而没有数据的情况

		TextureWrapType		wrapType;			///< 纹理环绕模式
		TextureFilterType	minFilterType;		///< 纹理缩小过滤
		TextureFilterType	magFilterType;		///< 纹理放大过滤

		TextureGenMode		genMode;			///< 自动纹理坐标生成模式

		// Eye coordinate
		//bool				useEyeSpaceTex;
		Matrix4				eyeSpaceMatrix;		///< 视点空间矩阵

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

		// 初始化RenderState的初值
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

	// 引擎纹理基类
	// 引擎纹理用于资源的索引，并记录设备纹理的指针
	class BaseTexture : public Resource
	{
		friend class TextureManager;
	public:
		BaseTexture(const String& filename);

		// ----- Overwrite Resource
		//bool Load();
		void UnloadImpl();

		// 获取设备纹理
		IDeviceTexture* GetDeviceTexture() const { return m_DeviceTexture; }

	protected:
		IDeviceTexture*		m_DeviceTexture;		///< 设备纹理

	};

	class Texture2D : public BaseTexture
	{
	public:
		Texture2D(const String& filename);

		// ----- Texture2D Methods

		// 创建纹理
		void Create(unsigned int width, unsigned int height, unsigned int bpp, unsigned char* data);

		// 修改纹理内容
		void ModifyRectData(int xoffset, int yoffset, int width, int height, void* data);
	protected:
		// ----- Overwrite Resource
		bool LoadImpl();

		// ----- Texture2D Methods

	};

	// 立方体纹理的位置
	enum CubeMapSide
	{
		CUBE_MAP_POS_X = 0,
		CUBE_MAP_NEG_X,
		CUBE_MAP_POS_Y,
		CUBE_MAP_NEG_Y,
		CUBE_MAP_POS_Z,
		CUBE_MAP_NEG_Z,
	};

	// 立方体纹理
	class CubeTexture : public BaseTexture
	{
		//friend class TextureManager;
	public:
		CubeTexture(const String& filename);

		// 创建纹理
		void Create(unsigned int width, unsigned int height, unsigned int bpp, unsigned char* data, int cubeSide);

	protected:
		// ----- Overwrite Resource
		bool LoadImpl();
	};
}

#endif
