//-----------------------------------------------------------------------------------
/// BaseUIObject.cpp UI对象基类，包含UI对象基本方法的实现
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#include "BaseUIObject.h"
#include "Engine.h"
#include "System.h"

BaseUIObject::BaseUIObject()
: m_ZOrder(100),		// 默认顺序为100
  m_Texture(NULL)
{
	// 默认水平方向左对齐，垂直方向顶对齐
	m_VAlignType = VALIGN_TOP;
	m_Top = 0;
	m_Height = 0;
	m_HAlignType = HALIGN_LEFT;
	m_Left = 0;
	m_Width = 0;
}

void BaseUIObject::Update(unsigned long deltaTime)
{

}

void BaseUIObject::Render()
{
	int left, right, top, bottom;

	RenderWindowParam* param = System::Instance().GetRenderWindowParameters();
	int screenWidth = param->width;
	int screenHeight = param->height;

	switch (m_VAlignType)
	{
	case VALIGN_TOP:
		top = m_Top;
		bottom = m_Top + m_Height;
		break;

	case VALIGN_MIDDLE:
		top = (screenHeight - m_Height) / 2;
		bottom = (screenHeight + m_Height) / 2;
		break;

	case VALIGN_BOTTOM:
		top = screenHeight - m_Bottom - m_Height;
		bottom = screenHeight - m_Bottom;
		break;

	default:
		AssertFatal(0, "BaseUIObject::Render() : Invalid VerticalAlignType.");
	}

	switch (m_HAlignType)
	{
	case HALIGN_LEFT:
		left = m_Left;
		right = m_Left + m_Width;
		break;

	case HALIGN_MIDDLE:
		left = (screenWidth - m_Width) / 2;
		right = (screenWidth + m_Width) / 2;
		break;

	case HALIGN_RIGHT:
		left = screenWidth - m_Right - m_Width;
		right = screenWidth - m_Right;
		break;

	default:
		AssertFatal(0, "BaseUIObject::Render() : Invalid HorizontalAlignType.");
	}

	renderer->RenderScreenQuad(left, top, right, bottom, m_Texture);
}

void BaseUIObject::SetZOrder(int z)
{
	m_ZOrder = z;
}
