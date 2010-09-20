//-----------------------------------------------------------------------------------
/// ImageUIControl.cpp Í¼ÏñUI¿Ø¼þ
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------

#include "ImageUIControl.h"
#include "Engine.h"
#include "System.h"
#include "Renderer.h"

namespace Gen
{
	ImageUIControl::ImageUIControl()
		: m_Texture(NULL),
		m_Color(1.0f, 1.0f, 1.0f)
	{
		m_VertexBuffer = Renderer::Instance().BuildVertexBuffer();
		m_IndexBuffer = Renderer::Instance().BuildIndexBuffer();

		unsigned int index[] = { 0, 2, 1, 0, 3, 2 };
		m_IndexBuffer->CreateBuffer(index, 2);
	}

	ImageUIControl::~ImageUIControl()
	{
		SAFE_DELETE(m_VertexBuffer);
		SAFE_DELETE(m_IndexBuffer);
	}

	void ImageUIControl::Render()
	{
		if (!m_Texture) return;

		UpdateControlSizeValue();

		m_VertexBuffer->Clear();

		float vertexArray[] = { m_Left, m_Bottom, -1.0f,
								m_Left, m_Top, -1.0f,
								m_Right, m_Top, -1.0f,
								m_Right, m_Bottom, -1.0f };

		float texcoordArray[] = { 0.0f, 0.0f,
								  0.0f, 1.0f,
								  1.0f, 1.0f,
								  1.0f, 0.0f };

		m_VertexBuffer->CreateBuffer(VFormat_Position|VFormat_Texcoord0,
									 vertexArray,
									 NULL,
									 NULL,
									 texcoordArray,
									 4);

		Renderer::Instance().SetModelMatrix(Matrix4::IDENTITY);
		Renderer::Instance().BindTextureRenderState(m_Texture->GetDeviceTexture());
		Renderer::Instance().RenderPrimitives(m_VertexBuffer, m_IndexBuffer, PRIM_TRIANGLES);
	}

	void ImageUIControl::SetTexture(BaseTexture* texture)
	{
		texture->Trigger();
		m_Texture = texture;
	}

}
