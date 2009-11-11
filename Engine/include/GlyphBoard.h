//-----------------------------------------------------------------------------------
/// GlyphBoard.h 字模板，将一批相近的文字调用保存在同一个GlyphBoard中
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
