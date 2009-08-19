//-----------------------------------------------------------------------------------
/// ITexture.h 纹理接口，提供纹理操作的公共方法
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#ifndef _ITEXTURE_H
#define _ITEXTURE_H

#include "Matrix4.h"
#include "EString.h"
#include "IGpuProgram.h"

using namespace std;

// 纹理环绕模式
// 超出纹理边缘以后的采样方式
enum TextureWrapType
{
	WRAP_TYPE_CLAMP,
	WRAP_TYPE_REPEAT,
	WRAP_TYPE_CLAMP_TO_EDGE,
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

// 纹理混合因子
enum TextureBlendFactor
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

class ITexture;

// 纹理渲染状态
typedef struct texRenderState
{
	ITexture*			texture;			///< 纹理
	TextureWrapType		wrapType;			///< 纹理环绕模式
	TextureFilterType	minFilterType;		///< 纹理缩小过滤
	TextureFilterType	magFilterType;		///< 纹理放大过滤

	TextureGenMode		genMode;			///< 自动纹理坐标生成模式

	// Eye coordinate
	//bool				useEyeSpaceTex;
	Matrix4				eyeSpaceMatrix;		///< 视点空间矩阵

	// Blending
	bool				useBlending;		///< 采用纹理混合
	TextureBlendFactor	srcBlendFactor;		///< 源混合因子
	TextureBlendFactor	dstBlendFactor;		///< 目标混合因子

	TextureEnvMode		envMode;

	texRenderState()
	{
		InitValues();
	}

	texRenderState(ITexture* tex)
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
		useBlending = true;
		srcBlendFactor = BLEND_FACTOR_SRC_ALPHA;
		dstBlendFactor = BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;

		envMode = ENV_MODE_MODULATE;
	}

} texRenderState_t;

class ITexture
{
public:
	virtual ~ITexture() {}

	// 纹理修改相关
	//virtual void Create(unsigned int width, unsigned int height, unsigned int bpp, unsigned char* data) = 0;

	// 修改纹理上指定的一个矩形区域数据
	virtual void ModifyRectData(int xoffset, int yoffset, int width, int heigh, void* data) = 0;

	// 使用一个纹理
	virtual void BindTexture() = 0;

	// TODO: 添加纹理相关的方法，如获取纹理尺寸等
	virtual unsigned int GetWidth() const = 0;
	virtual unsigned int GetHeight() const = 0;
	virtual unsigned int GetBpp() const = 0;

	virtual String GetName() const = 0;

	// for gl only
	virtual int GetTarget() const = 0;

	// 纹理过滤
};

class BaseTexture : public ITexture
{
public:
	~BaseTexture() {}

	unsigned int GetWidth() const { return m_Width; }
	unsigned int GetHeight() const { return m_Height; }
	unsigned int GetBpp() const { return m_Bpp; }

	String GetName() const { return *m_Name; }
	void SetName(const String* name) { m_Name = name; }

protected:
	const String*	m_Name;

	unsigned int	m_Width;
	unsigned int	m_Height;
	unsigned int	m_Bpp;
};

#endif
