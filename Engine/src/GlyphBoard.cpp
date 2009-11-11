//-----------------------------------------------------------------------------------
/// GlyphBoard.cpp ������GlyphBoard���ʵ��
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#include "GlyphBoard.h"

#include "Engine.h"

namespace Gen
{
#define GLYPHBOARD_SIZE 512

	GlyphBoard::GlyphBoard()
		: m_Texture(NULL)
	{
		// ����һ������������������

		static unsigned char empty[GLYPHBOARD_SIZE * GLYPHBOARD_SIZE * 4] = { 0 };

		renderer->BuildTexture("GlyphBoard", GLYPHBOARD_SIZE, GLYPHBOARD_SIZE, 32, empty);
	}

	GlyphBoard::~GlyphBoard()
	{
	}
}
