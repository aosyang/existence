//-----------------------------------------------------------------------------------
/// ImageUIControl.h 图像UI控件
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------

#ifndef _IMAGEUICONTROL_H
#define _IMAGEUICONTROL_H

#include "BaseUIObject.h"

class ImageUIControl : public BaseUIObject
{
public:
	ImageUIControl();
	//~ImageUIControl();

	void Render();

	void SetTexture(ITexture* texture) { m_Texture = texture; }
	ITexture* GetTexture() const { return m_Texture; }

	void SetColor(const Color4f& color) { m_Color = color; }
	Color4f GetColor() const { return m_Color; }
protected:

	ITexture*			m_Texture;		///< 控件使用的纹理

	Color4f				m_Color;		///< 控件颜色

};

#endif
