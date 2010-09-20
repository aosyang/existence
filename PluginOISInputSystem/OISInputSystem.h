//-----------------------------------------------------------------------------------
/// OISInputSystem.h OIS输入系统插件
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 - 2010 by Mwolf
//-----------------------------------------------------------------------------------

#ifndef _OISINPUTSYSTEM_H
#define _OISINPUTSYSTEM_H

#include "Input.h"
#include "OIS/OIS.h"

namespace Gen
{
#define KEY_DOWN 0
#define KEY_UP 1

	class OISInputSystem :	public IInputSystem,
							public OIS::KeyListener,
							public OIS::MouseListener,
							public OIS::JoyStickListener
	{
	public:
		OISInputSystem();

		/// @copydoc IInputSystem::Initialize(RenderWindowHandle)
		void Initialize(RenderWindowHandle rw);

		/// @copydoc IInputSystem::Shutdown()
		void Shutdown();

		/// @copydoc IInputSystem::CaptureDevice()
		void CaptureDevice();

		/// @copydoc IInputSystem::ResizeWindow(unsigned int, unsigned int)
		void ResizeWindow(unsigned int width, unsigned int height);

		/// @copydoc IInputSystem::SetEventHandler(IInputEventHandler*)
		void SetEventHandler(IInputEventHandler* handler);

		bool GetKeyDown(KeyCode key);

		bool GetMouseButtonDown(MouseButtonID ButtonCode);
		int GetMousePosX() { return m_MouseX; }
		int GetMousePosY() { return m_MouseY; }

		int GetJoyStickAbs(int axis);

	private:

		// 从OIS键码转换为引擎键码
		KeyCode FromOISKeyCode(OIS::KeyCode key) const;
		OIS::KeyCode ToOISKeyCode(KeyCode key) const;

		// OIS事件

		// 键盘事件
		bool keyPressed( const OIS::KeyEvent &arg );
		bool keyReleased( const OIS::KeyEvent &arg );

		// 鼠标事件
		bool mouseMoved( const OIS::MouseEvent &arg );
		bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
		bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );

		// 控制器事件
		bool buttonPressed( const OIS::JoyStickEvent &arg, int button );
		bool buttonReleased( const OIS::JoyStickEvent &arg, int button );
		bool axisMoved( const OIS::JoyStickEvent &arg, int axis );
	private:
		bool				m_Initialized;

		OIS::InputManager*	m_InputManager;
		OIS::Keyboard*		m_Keyboard;
		OIS::Mouse*			m_Mouse;
		OIS::JoyStick*		m_JoyStick;

		bool				m_MouseButtonState[MAX_MB_NUM];
		bool				m_JoyStickButton[24];

		int					m_JoyStickAxis[6];
		float				m_DeadZone;

		int					m_MouseX, m_MouseY;

		IInputEventHandler*	m_EventHandler;
	};

	extern "C" DLLEXPORT IPlugin* CreatePluginInstance();
}

#endif
