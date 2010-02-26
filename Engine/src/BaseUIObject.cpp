//-----------------------------------------------------------------------------------
/// BaseUIObject.cpp UI������࣬����UI�������������ʵ��
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#include "BaseUIObject.h"
#include "Debug.h"
#include "System.h"

namespace Gen
{
	BaseUIObject::BaseUIObject()
		: m_ZOrder(100),		// Ĭ��˳��Ϊ100
		m_IsVisible(true),
		m_SizingType(SIZING_TYPE_HEIGHT_SCALE)
	{
		// Ĭ��ˮƽ��������룬��ֱ���򶥶���
		m_VAlignType = VALIGN_TOP;
		m_VBorderSize = 0.0f;
		m_Height = 0.5f;
		m_HAlignType = HALIGN_LEFT;
		m_HBorderSize = 0.0f;
		m_Width = 0.5f;
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

	void BaseUIObject::UpdateControlSizeValue()
	{
		// �߶ȱ���ģʽ��
		// ���·�����Ϊ(-1.0f, -1.0f)
		// ���Ϸ�����Ϊ(-1.0f, 1.0f)
		// ���Ϸ�����Ϊ(1.0f, 1.0f)

		unsigned int	width = System::Instance().GetRenderWindowParameters()->width,
						height = System::Instance().GetRenderWindowParameters()->height;

		float aspect = (float)width/height;

		if (m_SizingType==SIZING_TYPE_PIXEL)
		{
			// TODO: ����ģʽ
		}
		else if (m_SizingType==SIZING_TYPE_HEIGHT_SCALE)
		{
			// ��ֱ����ģʽ
			switch (m_VAlignType)
			{
			case VALIGN_TOP:
				//m_Top = m_VBorderSize;
				m_Top = 1.0f - m_VBorderSize * 2;
				break;

			case VALIGN_MIDDLE:
				m_Top = m_Height;
				break;

			case VALIGN_BOTTOM:
				m_Top = -1.0f + (m_VBorderSize + m_Height) * 2;
				break;
			}
			m_Bottom = m_Top - m_Height * 2;


			switch (m_HAlignType)
			{
			case HALIGN_LEFT:
				m_Left = (m_HBorderSize - (aspect - 1.0f) * 0.5f) * 2 - 1.0f;
				break;

			case HALIGN_MIDDLE:
				m_Left = - m_Width;
				break;

			case HALIGN_RIGHT:
				m_Left = 1.0f - (m_HBorderSize + m_Width - (aspect- 1.0f) * 0.5f) * 2;
				break;
			}
			m_Right = m_Left + m_Width * 2;

		}
	}

}
