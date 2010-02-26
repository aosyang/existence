//-----------------------------------------------------------------------------------
/// ImageUIControl.h ͼ��UI�ؼ�
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------

#ifndef _IMAGEUICONTROL_H
#define _IMAGEUICONTROL_H

#include "BaseUIObject.h"

namespace Gen
{
	class IVertexBuffer;
	class IIndexBuffer;

	class ImageUIControl : public BaseUIObject
	{
	public:
		ImageUIControl();
		~ImageUIControl();

		void Render();

		void SetTexture(BaseTexture* texture);
		BaseTexture* GetTexture() const { return m_Texture; }

		void SetColor(const Color4f& color) { m_Color = color; }
		Color4f GetColor() const { return m_Color; }
	protected:
		BaseTexture*			m_Texture;		///< �ؼ�ʹ�õ�����

		Color4f					m_Color;		///< �ؼ���ɫ

		IVertexBuffer*			m_VertexBuffer;
		IIndexBuffer*			m_IndexBuffer;
	};
}

#endif
