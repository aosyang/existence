//-----------------------------------------------------------------------------------
/// DebugRenderer.h ������Ϣ��Ⱦ��
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
	///		������Ϣ��Ⱦ��
	///	@par
	///		��3D�ռ�����Ⱦ�߿�ȸ�����Ϣ
	class DebugRenderer : public Singleton<DebugRenderer>
	{
		friend class Singleton<DebugRenderer>;
		friend class SceneGraph;
	public:

		/// @brief
		///	��ʼ��������Ϣ��Ⱦ��
		void Initialize();

		/// @brief
		///	�رյ�����Ϣ��Ⱦ��
		void Shutdown();

		/// @brief
		///	�����߶�
		/// @param begin
		///		�߶���ʼ��������
		/// @param end
		///		�߶ν�����������
		/// @param color
		///		�߶���ɫ
		/// @remarks
		///		����������Ƶ��߶β��������ȼ�⣬ֱ�ӻ��Ƶ���Ļ
		void DrawLine(const Vector3f& begin, const Vector3f& end, const Color4f& color = Color4f(1.0f, 1.0f, 1.0f));

		/// @brief
		///	�����߶�()
		/// @param begin
		///		�߶���ʼ��������
		/// @param end
		///		�߶ν�����������
		/// @param color
		///		�߶���ɫ
		void DrawLineWithDepthTest(const Vector3f& begin, const Vector3f& end, const Color4f& color = Color4f(1.0f, 1.0f, 1.0f));

		/// @brief
		///	���Ʒ���
		/// @param vMin
		///		��xyz��������С������ɵ���ά����
		/// @param vMax
		///		��xyz���������������ɵ���ά����
		/// @param color
		///		��Ⱦ��ɫ
		/// @param transform
		///		����ռ�任
		/// @param depth
		///		�Ի���ͼ�ν�����Ȳ���
		/// @remarks
		///		����������棬ͳһ���춥�㻺��
		void DrawBox(const Vector3f& vMin, const Vector3f& vMax, const Color4f& color = Color4f(1.0f, 1.0f, 1.0f), const Matrix4& transform = Matrix4::IDENTITY, bool depth = false);

		/// @brief
		///	���������߿�
		/// @param point
		///		������������
		/// @param radius
		///		����뾶
		/// @param segment
		///		����Բ�ֶܷ�
		void DrawSphere(const Vector3f& point, float radius, const Color4f& color = Color4f(1.0f, 1.0f, 1.0f), unsigned int segment = 24);

		static void ToggleSceneObjectDebugRender(bool toggle);

		void ClearBuffer();

	protected:
		/// @brief
		///	���Ƶ���Ļ��
		void RenderToScreen();

		void RenderToScreenWithDepthTest();
	private:
		/// ���캯��
		DebugRenderer();

		IVertexBuffer*			m_VertexBuffer;		///< ���㻺��

		std::vector<float>			m_VertexPosition;	///< ������������
		std::vector<float>			m_VertexColor;		///< ������ɫ����

		std::vector<float>			m_VertexPositionDepth;	///< ������������(����Ȳ���)
		std::vector<float>			m_VertexColorDepth;		///< ������ɫ����(����Ȳ���)
	};
}

#endif
