//-----------------------------------------------------------------------------------
/// DebugRenderer.cpp 调试信息渲染器实现
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 - 2010 by Mwolf
//-----------------------------------------------------------------------------------

#include "DebugRenderer.h"
#include "Renderer.h"
#include "SceneObject.h"

namespace Gen
{
	DebugRenderer::DebugRenderer()
		: m_VertexBuffer(NULL)
	{
	}

	void DebugRenderer::Initialize()
	{
		m_VertexBuffer = Renderer::Instance().BuildVertexBuffer();
	}

	void DebugRenderer::Shutdown()
	{
		SAFE_DELETE(m_VertexBuffer);
	}

	void DebugRenderer::DrawLine(const Vector3f& begin, const Vector3f& end, const Color4f& color)
	{
		m_VertexPosition.push_back(begin.x);
		m_VertexPosition.push_back(begin.y);
		m_VertexPosition.push_back(begin.z);

		m_VertexPosition.push_back(end.x);
		m_VertexPosition.push_back(end.y);
		m_VertexPosition.push_back(end.z);

		m_VertexColor.push_back(color.r);
		m_VertexColor.push_back(color.g);
		m_VertexColor.push_back(color.b);
		m_VertexColor.push_back(color.a);

		m_VertexColor.push_back(color.r);
		m_VertexColor.push_back(color.g);
		m_VertexColor.push_back(color.b);
		m_VertexColor.push_back(color.a);
	}

	void DebugRenderer::DrawLineWithDepthTest(const Vector3f& begin, const Vector3f& end, const Color4f& color)
	{
		m_VertexPositionDepth.push_back(begin.x);
		m_VertexPositionDepth.push_back(begin.y);
		m_VertexPositionDepth.push_back(begin.z);

		m_VertexPositionDepth.push_back(end.x);
		m_VertexPositionDepth.push_back(end.y);
		m_VertexPositionDepth.push_back(end.z);

		m_VertexColorDepth.push_back(color.r);
		m_VertexColorDepth.push_back(color.g);
		m_VertexColorDepth.push_back(color.b);
		m_VertexColorDepth.push_back(color.a);

		m_VertexColorDepth.push_back(color.r);
		m_VertexColorDepth.push_back(color.g);
		m_VertexColorDepth.push_back(color.b);
		m_VertexColorDepth.push_back(color.a);
	}


	void DebugRenderer::DrawBox(const Vector3f& vMin, const Vector3f& vMax, const Color4f& color, const Matrix4& transform, bool depth)
	{
		Vector3f v[8] = 
		{
			transform * Vector3f(vMin.x, vMin.y, vMin.z),
			transform * Vector3f(vMin.x, vMin.y, vMax.z),
			transform * Vector3f(vMin.x, vMax.y, vMin.z),
			transform * Vector3f(vMin.x, vMax.y, vMax.z),

			transform * Vector3f(vMax.x, vMin.y, vMin.z),
			transform * Vector3f(vMax.x, vMin.y, vMax.z),
			transform * Vector3f(vMax.x, vMax.y, vMin.z),
			transform * Vector3f(vMax.x, vMax.y, vMax.z),
		};

		if (depth)
		{
			DrawLineWithDepthTest(v[0], v[2], color);
			DrawLineWithDepthTest(v[2], v[3], color);
			DrawLineWithDepthTest(v[3], v[1], color);
			DrawLineWithDepthTest(v[1], v[0], color);

			DrawLineWithDepthTest(v[4], v[6], color);
			DrawLineWithDepthTest(v[6], v[7], color);
			DrawLineWithDepthTest(v[7], v[5], color);
			DrawLineWithDepthTest(v[5], v[4], color);

			DrawLineWithDepthTest(v[0], v[4], color);
			DrawLineWithDepthTest(v[2], v[6], color);
			DrawLineWithDepthTest(v[3], v[7], color);
			DrawLineWithDepthTest(v[1], v[5], color);
		}
		else
		{
			DrawLine(v[0], v[2], color);
			DrawLine(v[2], v[3], color);
			DrawLine(v[3], v[1], color);
			DrawLine(v[1], v[0], color);

			DrawLine(v[4], v[6], color);
			DrawLine(v[6], v[7], color);
			DrawLine(v[7], v[5], color);
			DrawLine(v[5], v[4], color);

			DrawLine(v[0], v[4], color);
			DrawLine(v[2], v[6], color);
			DrawLine(v[3], v[7], color);
			DrawLine(v[1], v[5], color);
		}
	}

	void DebugRenderer::DrawSphere(const Vector3f& point, float radius, const Color4f& color, unsigned int segment)
	{
		unsigned int i=0;
		Vector3f start, end(point.x, point.y + radius * cos(float(i)/segment * Math::k2Pi), point.z + radius * sin(float(i)/segment * Math::k2Pi));

		for (i=1; i<=segment; i++)
		{
			start = end;
			end = Vector3f(point.x, point.y + radius * cos(float(i)/segment * Math::k2Pi), point.z + radius * sin(float(i)/segment * Math::k2Pi));
			DrawLine(start, end, color);
		}

		i=0;
		end = Vector3f(point.x + radius * sin(float(i)/segment * Math::k2Pi), point.y + radius * cos(float(i)/segment * Math::k2Pi), point.z);
		for (i=1; i<=segment; i++)
		{
			start = end;
			end = Vector3f(point.x + radius * sin(float(i)/segment * Math::k2Pi), point.y + radius * cos(float(i)/segment * Math::k2Pi), point.z);
			DrawLine(start, end, color);
		}

		i=0;
		end = Vector3f(point.x + radius * sin(float(i)/segment * Math::k2Pi), point.y, point.z + radius * cos(float(i)/segment * Math::k2Pi));
		for (i=1; i<=segment; i++)
		{
			start = end;
			end = Vector3f(point.x + radius * sin(float(i)/segment * Math::k2Pi), point.y, point.z + radius * cos(float(i)/segment * Math::k2Pi));
			DrawLine(start, end, color);
		}

	}

	void DebugRenderer::ToggleSceneObjectDebugRender(bool toggle)
	{
		SceneObject::m_sDebugRender = toggle;
	}

	void DebugRenderer::ClearBuffer()
	{
		m_VertexPosition.clear();
		m_VertexColor.clear();
	}

	void DebugRenderer::RenderToScreen()
	{
		if (m_VertexBuffer)
			m_VertexBuffer->Clear();

		if (m_VertexPosition.size())
		{
			// 构造顶点缓冲
			m_VertexBuffer->CreateBuffer(VFormat_Position|VFormat_Color,
										 &m_VertexPosition[0],
										 NULL,
										 &m_VertexColor[0],
										 NULL,
										 m_VertexPosition.size() / 3);

			Renderer::Instance().SetModelMatrix(Matrix4::IDENTITY);
			Renderer::Instance().SetupMaterial(NULL);
			Renderer::Instance().RenderPrimitives(m_VertexBuffer, PRIM_LINES);
		}
	}

	void DebugRenderer::RenderToScreenWithDepthTest()
	{
		if (m_VertexBuffer)
			m_VertexBuffer->Clear();

		if (m_VertexPositionDepth.size())
		{
			// 构造顶点缓冲
			m_VertexBuffer->CreateBuffer(VFormat_Position|VFormat_Color,
										 &m_VertexPositionDepth[0],
										 NULL,
										 &m_VertexColorDepth[0],
										 NULL,
										 m_VertexPositionDepth.size() / 3);

			Renderer::Instance().SetModelMatrix(Matrix4::IDENTITY);
			Renderer::Instance().SetupMaterial(NULL);
			Renderer::Instance().RenderPrimitives(m_VertexBuffer, PRIM_LINES);
		}

		m_VertexPositionDepth.clear();
		m_VertexColorDepth.clear();
	}

}
