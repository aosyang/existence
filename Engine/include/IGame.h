//-----------------------------------------------------------------------------------
/// IGame.h 游戏接口，提供游戏类的公共方法
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
	///	游戏类接口
	/// @par
	///		提供了游戏类的公共方法接口，所有游戏类必须从这个接口继承
	class IGame
	{
	public:
		virtual ~IGame() {}

		/// @brief
		///	开始游戏
		/// @remarks
		///		派生类实现这个方法以完成需要在游戏循环开始之前完成的工作<br>
		///		如：构建场景对象
		virtual void StartGame() = 0;

		/// @brief
		///	关闭游戏
		/// @remarks
		///		派生类实现这个方法以完成需要在游戏退出之前完成的工作<br>
		///		如：删除场景中的内容
		virtual void Shutdown() = 0;

		/// @brief
		/// 游戏退出通知回调函数
		/// @returns
		///		true表示游戏将会正常退出；false将中断退出过程
		/// @remarks
		///		当引擎窗体关闭时，会向游戏发送退出通知，用户实现这个方法可以对程序退出进行控制
		virtual bool OnNotifyQuitting() = 0;

		/// @brief
		///	窗体缩放回调函数
		/// @param width
		///		窗体宽度
		/// @param height
		///		窗体高度
		///	@remarks
		///		当引擎窗体尺寸发生改变时，会调用这个方法，实现这个方法可以对窗体尺寸改变事件进行控制
		virtual void OnResizeWindow(unsigned int width, unsigned int height) {}

		/// @brief
		///	更新游戏
		/// @param deltaTime
		///		距上一次更新经过的时间(单位：毫秒)
		/// @remarks
		///		实现这个方法可以实现游戏中每帧必须完成的逻辑更新操作
		virtual void Update(unsigned long deltaTime) = 0;

		// TODO: Don't bother coders with this one...

		/// @brief
		///	渲染场景
		/// @remarks
		///		实现这个方法可以实现将游戏场景渲染到屏幕
		virtual void RenderScene() = 0;

	};
}

#endif
