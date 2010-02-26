//-----------------------------------------------------------------------------------
/// BaseUIObject.h UI������࣬����UI�������������ʵ��
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#ifndef _BASEUIOBJECT_H
#define _BASEUIOBJECT_H

#include "ISceneObject.h"
#include "Texture.h"

namespace Gen
{
	// ��ֱ���뷽ʽ
	enum VerticalAlignType
	{
		VALIGN_TOP,
		VALIGN_MIDDLE,
		VALIGN_BOTTOM
	};

	// ˮƽ���뷽ʽ
	enum HorizontalAlignType
	{
		HALIGN_LEFT,
		HALIGN_MIDDLE,
		HALIGN_RIGHT
	};

	// �ؼ��ߴ��ʾ��ʽ
	enum UIControlSizingType
	{
		SIZING_TYPE_PIXEL,				///< ��Ļ�ֱ���ģʽ
		SIZING_TYPE_HEIGHT_SCALE,		///< �߶ȱ���ģʽ
	};

	// ע�����гߴ�����Ļ��������Ϊ׼�����Ͻ�Ϊ(0, 0)��

	class BaseUIObject : public ISceneObject
	{
	public:
		BaseUIObject();

		// ----- Overwrite ISceneObject

		void Update(unsigned long deltaTime);
		void Render();
		void DebugRender() {}

		// ----- BaseUIObject Methods

		// �ؼ�����Ļ�ϵ�ǰ��˳��(С�����󣬴�����ǰ)
		// TODO: ��Ϊ�������󣬲ο�BaseSceneObject
		void SetZOrder(int z);
		int GetZOrder() const { return m_ZOrder; }

		void SetVerticalAlign(VerticalAlignType align) { m_VAlignType = align; }
		VerticalAlignType GetVerticalAlign() const { return m_VAlignType; }

		void SetHorizontalAlign(HorizontalAlignType align) { m_HAlignType = align; }
		HorizontalAlignType GetHorizontalAlign() const { return m_HAlignType; }

		// �����ߴ�
		void SetVBorderSize(float size) { m_VBorderSize = size; }
		float GetVBorderSize() const { return m_VBorderSize; }

		void SetHBorderSize(float size) { m_HBorderSize = size; }
		float GetHBorderSize() const { return m_HBorderSize; }

		void SetHeight(float height) { m_Height = height; }
		float GetHeight() const { return m_Height; }

		void SetWidth(float width) { m_Width = width; }
		float GetWidth() const { return m_Width; }

		// ���سߴ�
		void SetVPixelBorderSize(int size) { m_VPixelBorderSize = size; }
		int GetVPixelBorderSize() const { return m_VPixelBorderSize; }

		void SetHPixelBorderSize(int size) { m_HPixelBorderSize = size; }
		int GetHPixelBorderSize() const { return m_HPixelBorderSize; }

		void SetPixelHeight(int height) { m_PixelHeight = height; }
		int GetPixelHeight() const { return m_PixelHeight; }

		void SetPixelWidth(int width) { m_PixelWidth = width; }
		int GetPixelWidth() const { return m_PixelWidth; }

		void SetVisible(bool visible) { m_IsVisible = visible; }
		bool GetVisible() const { return m_IsVisible; }

	protected:
		// ������Ⱦ�ߴ�
		void UpdateControlSizeValue();

	protected:
		int m_ZOrder;						///< UIǰ��˳���ϵ

		float	m_VBorderSize;				///< ��ֱ��Ե�ߴ�(�����Ϸ����룬�൱��top�������·����룬�൱��bottom������ʱ��Ч)
		float	m_HBorderSize;				///< ˮƽ��Ե�ߴ�

		float	m_Width,					///< �ؼ����
				m_Height;					///< �ؼ��߶�

		int		m_VPixelBorderSize;			///< ��ֱ��Ե���سߴ�
		int		m_HPixelBorderSize;			///< ˮƽ��Ե���سߴ�

		int		m_PixelWidth,				///< �ؼ����
				m_PixelHeight;				///< �ؼ��߶�

		float	m_Left,
				m_Top,
				m_Right,
				m_Bottom;

		VerticalAlignType	m_VAlignType;	///< ��ֱ���뷽ʽ
		HorizontalAlignType	m_HAlignType;	///< ˮƽ���뷽ʽ

		bool	m_IsVisible;				///< �Ƿ�ɼ�

		UIControlSizingType		m_SizingType;
	};
}

#endif
