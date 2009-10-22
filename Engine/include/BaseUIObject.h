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
#include "ITexture.h"

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

	void SetHeight(int height) { m_Height = height; }
	int GetHeight() const { return m_Height; }

	void SetWidth(int width) { m_Width = width; }
	int GetWidth() const { return m_Width; }

	void SetLeft(int left) { m_Left = left; }
	int GetLeft() const { return m_Left; }

	void SetTop(int top) { m_Top = top; }
	int GetTop() const { return m_Top; }

	void SetVisible(bool visible) { m_IsVisible = visible; }
	bool GetVisible() const { return m_IsVisible; }

protected:
	int m_ZOrder;						///< UI前后顺序关系

	union {
		int m_Left;						///< 距屏幕左端距离
		int m_Right;					///< 距屏幕右端距离
	};

	union {
		int m_Top;						///< 距屏幕上端距离
		int m_Bottom;					///< 距屏幕下端距离
	};

	int m_Width,						///< 控件宽度
		m_Height;						///< 控件高度

	VerticalAlignType	m_VAlignType;	///< 垂直对齐方式
	HorizontalAlignType	m_HAlignType;	///< 水平对齐方式

	bool	m_IsVisible;				///< 是否可见
};

#endif
