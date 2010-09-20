//-----------------------------------------------------------------------------------
/// IGame.h ��Ϸ�ӿڣ��ṩ��Ϸ��Ĺ�������
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#ifndef _IGAME_H
#define _IGAME_H

namespace Gen
{
	/// @brief
	///	��Ϸ��ӿ�
	/// @par
	///		�ṩ����Ϸ��Ĺ��������ӿڣ�������Ϸ����������ӿڼ̳�
	class IGame
	{
	public:
		virtual ~IGame() {}

		/// @brief
		///	��ʼ��Ϸ
		/// @remarks
		///		������ʵ����������������Ҫ����Ϸѭ����ʼ֮ǰ��ɵĹ���<br>
		///		�磺������������
		virtual void StartGame() = 0;

		/// @brief
		///	�ر���Ϸ
		/// @remarks
		///		������ʵ����������������Ҫ����Ϸ�˳�֮ǰ��ɵĹ���<br>
		///		�磺ɾ�������е�����
		virtual void Shutdown() = 0;

		/// @brief
		/// ��Ϸ�˳�֪ͨ�ص�����
		/// @returns
		///		true��ʾ��Ϸ���������˳���false���ж��˳�����
		/// @remarks
		///		�����洰��ر�ʱ��������Ϸ�����˳�֪ͨ���û�ʵ������������ԶԳ����˳����п���
		virtual bool OnNotifyQuitting() = 0;

		/// @brief
		///	�������Żص�����
		/// @param width
		///		������
		/// @param height
		///		����߶�
		///	@remarks
		///		�����洰��ߴ緢���ı�ʱ����������������ʵ������������ԶԴ���ߴ�ı��¼����п���
		virtual void OnResizeWindow(unsigned int width, unsigned int height) {}

		/// @brief
		///	������Ϸ
		/// @param deltaTime
		///		����һ�θ��¾�����ʱ��(��λ������)
		/// @remarks
		///		ʵ�������������ʵ����Ϸ��ÿ֡������ɵ��߼����²���
		virtual void Update(unsigned long deltaTime) = 0;

		// TODO: Don't bother coders with this one...

		/// @brief
		///	��Ⱦ����
		/// @remarks
		///		ʵ�������������ʵ�ֽ���Ϸ������Ⱦ����Ļ
		virtual void RenderScene() = 0;

	};
}

#endif
