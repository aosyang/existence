//-----------------------------------------------------------------------------------
/// BaseUIObject.cpp UI对象基类，包含UI对象基本方法的实现
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#include "BaseUIObject.h"

BaseUIObject::BaseUIObject()
: m_ZOrder(100),		// 默认顺序为100
  m_IsVisible(true)
{
	// 默认水平方向左对齐，垂直方向顶对齐
	m_VAlignType = VALIGN_TOP;
	m_Top = 0;
	m_Height = 100;
	m_HAlignType = HALIGN_LEFT;
	m_Left = 0;
	m_Width = 100;
}

void BaseUIObject::Update(unsigned long deltaTime)
{

}

void BaseUIObject::Render()
{

}

void BaseUIObject::SetZOrder(int z)
{
	m_ZOrder = z;
}
