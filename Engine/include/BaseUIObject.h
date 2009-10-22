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
#include "ITexture.h"

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
	int m_ZOrder;						///< UIǰ��˳���ϵ

	union {
		int m_Left;						///< ����Ļ��˾���
		int m_Right;					///< ����Ļ�Ҷ˾���
	};

	union {
		int m_Top;						///< ����Ļ�϶˾���
		int m_Bottom;					///< ����Ļ�¶˾���
	};

	int m_Width,						///< �ؼ����
		m_Height;						///< �ؼ��߶�

	VerticalAlignType	m_VAlignType;	///< ��ֱ���뷽ʽ
	HorizontalAlignType	m_HAlignType;	///< ˮƽ���뷽ʽ

	bool	m_IsVisible;				///< �Ƿ�ɼ�
};

#endif
