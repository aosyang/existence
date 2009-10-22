//-----------------------------------------------------------------------------------
/// BaseUIObject.cpp UI������࣬����UI�������������ʵ��
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#include "BaseUIObject.h"

BaseUIObject::BaseUIObject()
: m_ZOrder(100),		// Ĭ��˳��Ϊ100
  m_IsVisible(true)
{
	// Ĭ��ˮƽ��������룬��ֱ���򶥶���
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
