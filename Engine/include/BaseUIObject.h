//-----------------------------------------------------------------------------------
/// BaseUIObject.h UI对象基类，包含UI对象基本方法的实现
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
	// 垂直对齐方式
	enum VerticalAlignType
	{
		VALIGN_TOP,
		VALIGN_MIDDLE,
		VALIGN_BOTTOM
	};

	// 水平对齐方式
	enum HorizontalAlignType
	{
		HALIGN_LEFT,
		HALIGN_MIDDLE,
		HALIGN_RIGHT
	};

	// 控件尺寸表示方式
	enum UIControlSizingType
	{
		SIZING_TYPE_PIXEL,				///< 屏幕分辨率模式
		SIZING_TYPE_HEIGHT_SCALE,		///< 高度比例模式
	};

	// 注：所有尺寸以屏幕像素坐标为准，左上角为(0, 0)点

	class BaseUIObject : public ISceneObject
	{
	public:
		BaseUIObject();

		// ----- Overwrite ISceneObject

		void Update(unsigned long deltaTime);
		void Render();
		void DebugRender() {}

		// ----- BaseUIObject Methods

		// 控件在屏幕上的前后顺序(小数靠后，大数靠前)
		// TODO: 改为大数靠后，参考BaseSceneObject
		void SetZOrder(int z);
		int GetZOrder() const { return m_ZOrder; }

		void SetVerticalAlign(VerticalAlignType align) { m_VAlignType = align; }
		VerticalAlignType GetVerticalAlign() const { return m_VAlignType; }

		void SetHorizontalAlign(HorizontalAlignType align) { m_HAlignType = align; }
		HorizontalAlignType GetHorizontalAlign() const { return m_HAlignType; }

		// 比例尺寸
		void SetVBorderSize(float size) { m_VBorderSize = size; }
		float GetVBorderSize() const { return m_VBorderSize; }

		void SetHBorderSize(float size) { m_HBorderSize = size; }
		float GetHBorderSize() const { return m_HBorderSize; }

		void SetHeight(float height) { m_Height = height; }
		float GetHeight() const { return m_Height; }

		void SetWidth(float width) { m_Width = width; }
		float GetWidth() const { return m_Width; }

		// 象素尺寸
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
		// 更新渲染尺寸
		void UpdateControlSizeValue();

	protected:
		int m_ZOrder;						///< UI前后顺序关系

		float	m_VBorderSize;				///< 垂直边缘尺寸(对于上方对齐，相当于top；对于下方对齐，相当于bottom；居中时无效)
		float	m_HBorderSize;				///< 水平边缘尺寸

		float	m_Width,					///< 控件宽度
				m_Height;					///< 控件高度

		int		m_VPixelBorderSize;			///< 垂直边缘象素尺寸
		int		m_HPixelBorderSize;			///< 水平边缘象素尺寸

		int		m_PixelWidth,				///< 控件宽度
				m_PixelHeight;				///< 控件高度

		float	m_Left,
				m_Top,
				m_Right,
				m_Bottom;

		VerticalAlignType	m_VAlignType;	///< 垂直对齐方式
		HorizontalAlignType	m_HAlignType;	///< 水平对齐方式

		bool	m_IsVisible;				///< 是否可见

		UIControlSizingType		m_SizingType;
	};
}

#endif
