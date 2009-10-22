//-----------------------------------------------------------------------------------
/// IGame.h ��Ϸ�ӿڣ��ṩ��Ϸ��Ĺ�������
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
	/// ��ϵͳ֪ͨ��Ϸ��Ҫ�˳�ʱ����
	/// 
	/// \returns
	/// true��ʾ��Ϸ���������˳���false���ж��˳�����
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
