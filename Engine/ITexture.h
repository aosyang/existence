//-----------------------------------------------------------------------------------
/// ITexture.h ����ӿڣ��ṩ��������Ĺ�������
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

// ������ģʽ
// ���������Ե�Ժ�Ĳ�����ʽ
enum TextureWrapType
{
	WRAP_TYPE_CLAMP,
	WRAP_TYPE_REPEAT,
	WRAP_TYPE_CLAMP_TO_EDGE,
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

// ����������
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

class ITexture;

// ������Ⱦ״̬
typedef struct texRenderState
{
	ITexture*			texture;			///< ����
	TextureWrapType		wrapType;			///< ������ģʽ
	TextureFilterType	minFilterType;		///< ������С����
	TextureFilterType	magFilterType;		///< ����Ŵ����

	TextureGenMode		genMode;			///< �Զ�������������ģʽ

	// Eye coordinate
	//bool				useEyeSpaceTex;
	Matrix4				eyeSpaceMatrix;		///< �ӵ�ռ����

	// Blending
	bool				useBlending;		///< ����������
	TextureBlendFactor	srcBlendFactor;		///< Դ�������
	TextureBlendFactor	dstBlendFactor;		///< Ŀ��������

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

	// ��ʼ��RenderState�ĳ�ֵ
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

	// �����޸����
	//virtual void Create(unsigned int width, unsigned int height, unsigned int bpp, unsigned char* data) = 0;

	// �޸�������ָ����һ��������������
	virtual void ModifyRectData(int xoffset, int yoffset, int width, int heigh, void* data) = 0;

	// ʹ��һ������
	virtual void BindTexture() = 0;

	// TODO: ���������صķ��������ȡ����ߴ��
	virtual unsigned int GetWidth() const = 0;
	virtual unsigned int GetHeight() const = 0;
	virtual unsigned int GetBpp() const = 0;

	virtual String GetName() const = 0;

	// for gl only
	virtual int GetTarget() const = 0;

	// �������
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
