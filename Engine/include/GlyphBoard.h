//-----------------------------------------------------------------------------------
/// GlyphBoard.h ��ģ�壬��һ����������ֵ��ñ�����ͬһ��GlyphBoard��
///
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#ifndef _GLYPHBOARD_H
#define _GLYPHBOARD_H

#include "ITexture.h"

namespace Gen
{
	class GlyphBoard
	{
	public:
		GlyphBoard();
		~GlyphBoard();

	private:

		ITexture*	m_Texture;
	};
}

#endif
