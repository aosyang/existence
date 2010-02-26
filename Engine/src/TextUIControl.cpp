//-----------------------------------------------------------------------------------
/// TextUIControl.cpp UI文本控件，可以显示文本
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------

#include "TextUIControl.h"
#include "Engine.h"
#include "Renderer.h"
#include "System.h"

namespace Gen
{
	TextUIControl::TextUIControl()
		: BaseUIObject(),
		  m_Font(NULL),
		  m_WText(NULL),
		  m_TextLength(1),
		  m_FontSize(0.005f),		// TODO: 这个比例有错误
		  m_WordSpacing(0),
		  m_LineSpacing(0),
		  m_Dirty(true),
		  m_CharacterPixelSize(16),
		  m_GlyphTexSize(64)
	{
		m_WText = new wchar_t[1];
		m_WText[0] = 0;
		
		m_GlyphTexture = static_cast<DeviceTexture2D*>(Renderer::Instance().BuildTexture2D());
		m_GlyphTexture->Create(64, 64, 32, NULL);

		m_VertexBuffer = Renderer::Instance().BuildVertexBuffer();
		m_IndexBuffer = Renderer::Instance().BuildIndexBuffer();

		SetFont("Arial Black");
	}

	TextUIControl::~TextUIControl()
	{
		SAFE_DELETE_ARRAY(m_WText);
		SAFE_DELETE(m_GlyphTexture);
		SAFE_DELETE(m_VertexBuffer);
		SAFE_DELETE(m_IndexBuffer);
	}

	void TextUIControl::Update(unsigned long deltaTime)
	{
	}

	void TextUIControl::Render()
	{
		if (m_Dirty)
		{
			RefreshCharacters();
			m_Dirty = false;
		}

		if (m_VertexBuffer && m_IndexBuffer)
		{
			Renderer::Instance().BindTextureRenderState(m_GlyphTexture);
			Renderer::Instance().RenderPrimitives(m_VertexBuffer,
												  m_IndexBuffer,
												  Matrix4::IDENTITY);
		}
	}

	void TextUIControl::SetFont(const String& fontName)
	{
		if (fontName==m_FontName) return;
		m_FontName = fontName;

		m_Font = FontManager::Instance().GetByName(m_FontName);
		if (m_Font) m_Font->Trigger();

		m_Dirty = true;
	}

	void TextUIControl::SetText(const String& text)
	{

		// 先将string转为wchat_t类型
#if defined __PLATFORM_WIN32
		m_TextLength = MultiByteToWideChar(CP_ACP, 0, text.Data(), -1, NULL, 0);
#elif defined __PLATFORM_LINUX
		m_TextLength = mbstowcs(NULL, text.Data(), 0);
#endif

		// 重新为wchar分配空间
		SAFE_DELETE_ARRAY(m_WText);
		m_WText = new wchar_t[m_TextLength];

#if defined __PLATFORM_WIN32
		MultiByteToWideChar(CP_ACP, 0, text.Data(), -1, m_WText, m_TextLength);
#elif defined __PLATFORM_LINUX
		mbstowcs(m_WText, text.Data(), m_TextLength);
#endif

		// 标记为需要更新
		m_Dirty = true;
	}

	void TextUIControl::RefreshCharacters()
	{
		// 清除原来的文本内容
		m_Characters.clear();
		m_WideCharacters.clear();
		m_CharGlyphInfo.clear();

		if (!m_Font) return;

		// 首先统计文本中使用到的字符
		for (unsigned int i=0; i<m_TextLength; i++)
		{
			m_WideCharacters.insert(m_WText[i]);
		}

		EnlargeGlyphTexture();

		unsigned int x = 0, y = 0;
		size_t cap = m_GlyphTexSize / m_CharacterPixelSize;
		m_VisibleCharCount = 0;

		// 纹理坐标象素偏移
		static float pixelOffset;
		pixelOffset = 0.5f / m_GlyphTexSize;

		for (unsigned int i=0; i<m_TextLength; i++)
		{
			CharGlyphInfo info;

			// 如果这个字符已经添加过，直接调用旧字符
			if (m_CharGlyphInfo.find(m_WText[i])==m_CharGlyphInfo.end())
			{
				if (FontManager::Instance().BuildTexture(m_Font, m_WText[i], m_CharacterPixelSize, x * m_CharacterPixelSize, y * m_CharacterPixelSize, m_GlyphTexture, &info))
				{
					// 计算字符的纹理坐标

					// 左下
					info.u1 = (float)x * m_CharacterPixelSize / m_GlyphTexSize;
					info.v1 = ((float)m_CharacterPixelSize - info.height + y * m_CharacterPixelSize) / m_GlyphTexSize - pixelOffset;

					// 右上
					info.u2 = info.u1 + (float)info.width / m_GlyphTexSize;
					info.v2 = (float)(y + 1) * m_CharacterPixelSize / m_GlyphTexSize - pixelOffset;

					x++;

					// 字符写满纹理的一行，换行
					if (x>=cap)
					{
						x = 0;
						y++;
					}


					// 终止符及换行符
					if (m_WText[i]==0 || m_WText[i]=='\n')
					{
						// 这两个字符将其高度设为0表示不绘制字模
						info.height = 0;
					}
				}

				m_CharGlyphInfo[info.wchar] = info;
			}
			else
			{
				// 获取已有字符信息
				info = m_CharGlyphInfo[m_WText[i]];
			}
			if (info.height!=0 && info.width!=0)
				m_VisibleCharCount++;

			m_Characters.push_back(info);
		}

		BuildVertexBuffer();
	}

	void TextUIControl::BuildVertexBuffer()
	{
		UpdateControlSizeValue();

		// 创建控件的顶点及索引缓冲
		float* vertexArray = new float[m_VisibleCharCount * 3 * 4];
		float* texcoordArray = new float[m_VisibleCharCount * 2 * 4];
		unsigned int* indexArray = new unsigned int[m_VisibleCharCount * 6];

		float vx = 0.0f, vy = 0.0f;
		int width = 0;

		unsigned int screenWidth = System::Instance().GetRenderWindowParameters()->width,
					 screenHeight = System::Instance().GetRenderWindowParameters()->height;

		float aspect = (float)screenWidth / screenHeight;
		int visibleCount = 0;

		for (unsigned int i=0; i<m_Characters.size(); i++)
		{
			CharGlyphInfo info = m_Characters[i];

			// 换行符
			if (info.wchar=='\n')
			{
				vx = 0.0f;
				vy -= m_CharacterPixelSize + m_LineSpacing;
				width = 0;

				continue;
			}

			if (width+info.width > (m_Right - m_Left) / m_FontSize)
			{
				vx = 0.0f;
				vy -= m_CharacterPixelSize + m_LineSpacing;
				width = 0;
			}

			if (info.width!=0 && info.height!=0)
			{
				float x1 = m_FontSize * (info.left + vx) + m_Left;
				float x2 = m_FontSize * (info.left + info.width + vx) + m_Left;
				float y1 = m_FontSize * (info.top - info.height + vy - m_CharacterPixelSize) + m_Top;
				float y2 = m_FontSize * (info.top + vy + - m_CharacterPixelSize) + m_Top;		// * 0.004f

				vertexArray[visibleCount * 12] = x1;
				vertexArray[visibleCount * 12 + 1] = y1;
				vertexArray[visibleCount * 12 + 2] = -1.0f;
				texcoordArray[visibleCount * 8] = info.u1;
				texcoordArray[visibleCount * 8 + 1] = info.v1;

				vertexArray[visibleCount * 12 + 3] = x1;
				vertexArray[visibleCount * 12 + 4] = y2;
				vertexArray[visibleCount * 12 + 5] = -1.0f;
				texcoordArray[visibleCount * 8 + 2] = info.u1;
				texcoordArray[visibleCount * 8 + 3] = info.v2;

				vertexArray[visibleCount * 12 + 6] = x2;
				vertexArray[visibleCount * 12 + 7] = y2;
				vertexArray[visibleCount * 12 + 8] = -1.0f;
				texcoordArray[visibleCount * 8 + 4] = info.u2;
				texcoordArray[visibleCount * 8 + 5] = info.v2;

				vertexArray[visibleCount * 12 + 9] = x2;
				vertexArray[visibleCount * 12 + 10] = y1;
				vertexArray[visibleCount * 12 + 11] = -1.0f;
				texcoordArray[visibleCount * 8 + 6] = info.u2;
				texcoordArray[visibleCount * 8 + 7] = info.v1;

				indexArray[visibleCount * 6] = 0 + visibleCount * 4;
				indexArray[visibleCount * 6 + 1] = 2 + visibleCount * 4;
				indexArray[visibleCount * 6 + 2] = 1 + visibleCount * 4;
				indexArray[visibleCount * 6 + 3] = 0 + visibleCount * 4;
				indexArray[visibleCount * 6 + 4] = 3 + visibleCount * 4;
				indexArray[visibleCount * 6 + 5] = 2 + visibleCount * 4;

				visibleCount++;
			}

			vx += info.advance_x;
			width += info.advance_x;
		}

		if (m_VertexBuffer) m_VertexBuffer->Clear();
		m_VertexBuffer->CreateBuffer(VFormat_Position|VFormat_Texcoord0,
									 vertexArray,
									 NULL,
									 NULL,
									 texcoordArray,
									 4 * m_VisibleCharCount);

		if (m_IndexBuffer) m_IndexBuffer->Clear();
		m_IndexBuffer->CreateBuffer(indexArray, 2 * m_VisibleCharCount);

		SAFE_DELETE_ARRAY(vertexArray);
		SAFE_DELETE_ARRAY(texcoordArray);
		SAFE_DELETE_ARRAY(indexArray);
	}

	void TextUIControl::EnlargeGlyphTexture()
	{
		// 每行每列字符容量
		size_t cap = m_GlyphTexSize / m_CharacterPixelSize;
		size_t wcharCount = m_WideCharacters.size();

		// 容量足以装下目前文本，返回
		if (cap * cap >= wcharCount) return;

		// 扩容，直到能够容纳所有字符
		while (cap * cap < wcharCount)
		{
			m_GlyphTexSize <<= 1;
			cap = m_GlyphTexSize / m_CharacterPixelSize;
		}

		// 重新创建字模纹理
		SAFE_DELETE(m_GlyphTexture);
		m_GlyphTexture = static_cast<DeviceTexture2D*>(Renderer::Instance().BuildTexture2D());
		m_GlyphTexture->Create(m_GlyphTexSize, m_GlyphTexSize, 32, NULL);
	}
}
