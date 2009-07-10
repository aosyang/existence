//-----------------------------------------------------------------------------------
/// IGame.h 游戏接口，提供游戏类的公共方法
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#ifndef _IGAME_H
#define _IGAME_H

class IGame
{
public:
	virtual ~IGame() {}

	virtual void StartGame() = 0;
	virtual void Shutdown() = 0;

	//-----------------------------------------------------------------------------------
	/// \brief
	/// 当系统通知游戏需要退出时调用
	/// 
	/// \returns
	/// true表示游戏将会正常退出；false将中断退出过程
	//-----------------------------------------------------------------------------------
	virtual bool OnNotifyQuitting() = 0;

	virtual void OnKeyPressed(unsigned int key) = 0;
	virtual void OnKeyReleased(unsigned int key) = 0;

	virtual void OnMousePressed(unsigned int id) {}
	virtual void OnMouseReleased(unsigned int id) {}

	// TODO: to be pure virtual...
	virtual void OnResizeWindow(unsigned int width, unsigned int height) {}

	virtual void Update(unsigned long deltaTime) = 0;

	// TODO: Don't bother coders with this one...
	virtual void RenderScene() = 0;

};

#endif
