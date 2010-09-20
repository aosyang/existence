//-----------------------------------------------------------------------------------
/// DebugRenderer.h 调试信息渲染器
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 - 2010 by Mwolf
//-----------------------------------------------------------------------------------
#ifndef _DEBUGRENDERER_H
#define _DEBUGRENDERER_H

#include "Singleton.h"
#include "Color4f.h"
#include "Vector3f.h"
#include "Matrix4.h"
#include "IVertexBuffer.h"
#include <vector>



namespace Gen
{
	class SceneGraph;

	/// @brief
	///		调试信息渲染器
	///	@par
	///		在3D空间中渲染线框等辅助信息
	class DebugRenderer : public Singleton<DebugRenderer>
	{
		friend class Singleton<DebugRenderer>;
		friend class SceneGraph;
	public:

		/// @brief
		///	初始化调试信息渲染器
		void Initialize();

		/// @brief
		///	关闭调试信息渲染器
		void Shutdown();

		/// @brief
		///	绘制线段
		/// @param begin
		///		线段起始世界坐标
		/// @param end
		///		线段结束世界坐标
		/// @param color
		///		线段颜色
		/// @remarks
		///		这个方法绘制的线段不会进行深度检测，直接绘制到屏幕
		void DrawLine(const Vector3f& begin, const Vector3f& end, const Color4f& color = Color4f(1.0f, 1.0f, 1.0f));

		/// @brief
		///	绘制线段()
		/// @param begin
		///		线段起始世界坐标
		/// @param end
		///		线段结束世界坐标
		/// @param color
		///		线段颜色
		void DrawLineWithDepthTest(const Vector3f& begin, const Vector3f& end, const Color4f& color = Color4f(1.0f, 1.0f, 1.0f));

		/// @brief
		///	绘制方盒
		/// @param vMin
		///		由xyz方向上最小坐标组成的三维向量
		/// @param vMax
		///		由xyz方向上最大坐标组成的三维向量
		/// @param color
		///		渲染颜色
		/// @param transform
		///		世界空间变换
		/// @param depth
		///		对绘制图形进行深度测试
		/// @remarks
		///		将绘制命令储存，统一构造顶点缓冲
		void DrawBox(const Vector3f& vMin, const Vector3f& vMax, const Color4f& color = Color4f(1.0f, 1.0f, 1.0f), const Matrix4& transform = Matrix4::IDENTITY, bool depth = false);

		/// @brief
		///	绘制球体线框
		/// @param point
		///		球心世界坐标
		/// @param radius
		///		球体半径
		/// @param segment
		///		球体圆周分段
		void DrawSphere(const Vector3f& point, float radius, const Color4f& color = Color4f(1.0f, 1.0f, 1.0f), unsigned int segment = 24);

		static void ToggleSceneObjectDebugRender(bool toggle);

		void ClearBuffer();

	protected:
		/// @brief
		///	绘制到屏幕上
		void RenderToScreen();

		void RenderToScreenWithDepthTest();
	private:
		/// 构造函数
		DebugRenderer();

		IVertexBuffer*			m_VertexBuffer;		///< 顶点缓冲

		std::vector<float>			m_VertexPosition;	///< 顶点坐标数组
		std::vector<float>			m_VertexColor;		///< 顶点颜色数组

		std::vector<float>			m_VertexPositionDepth;	///< 顶点坐标数组(有深度测试)
		std::vector<float>			m_VertexColorDepth;		///< 顶点颜色数组(有深度测试)
	};
}

#endif
