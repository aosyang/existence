//-----------------------------------------------------------------------------------
/// Input.h 键盘、鼠标、控制器输入管理
///
/// File Encoding : GB2312
///
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#ifndef _INPUT_H
#define _INPUT_H


#include "Platform.h"
#include "Singleton.h"
#include "IPlugin.h"

namespace Gen
{
	// TODO: 使用自己定义的KeyCode替代OIS，并且暴露给用户

	//class Input;
	class InputHandler;

	// This comes from OIS...
	enum KeyCode
	{
		KC_UNASSIGNED  = 0x00,
		KC_ESCAPE      = 0x01,
		KC_1           = 0x02,
		KC_2           = 0x03,
		KC_3           = 0x04,
		KC_4           = 0x05,
		KC_5           = 0x06,
		KC_6           = 0x07,
		KC_7           = 0x08,
		KC_8           = 0x09,
		KC_9           = 0x0A,
		KC_0           = 0x0B,
		KC_MINUS       = 0x0C,    // - on main keyboard
		KC_EQUALS      = 0x0D,
		KC_BACK        = 0x0E,    // backspace
		KC_TAB         = 0x0F,
		KC_Q           = 0x10,
		KC_W           = 0x11,
		KC_E           = 0x12,
		KC_R           = 0x13,
		KC_T           = 0x14,
		KC_Y           = 0x15,
		KC_U           = 0x16,
		KC_I           = 0x17,
		KC_O           = 0x18,
		KC_P           = 0x19,
		KC_LBRACKET    = 0x1A,
		KC_RBRACKET    = 0x1B,
		KC_RETURN      = 0x1C,    // Enter on main keyboard
		KC_LCONTROL    = 0x1D,
		KC_A           = 0x1E,
		KC_S           = 0x1F,
		KC_D           = 0x20,
		KC_F           = 0x21,
		KC_G           = 0x22,
		KC_H           = 0x23,
		KC_J           = 0x24,
		KC_K           = 0x25,
		KC_L           = 0x26,
		KC_SEMICOLON   = 0x27,
		KC_APOSTROPHE  = 0x28,
		KC_GRAVE       = 0x29,    // accent
		KC_LSHIFT      = 0x2A,
		KC_BACKSLASH   = 0x2B,
		KC_Z           = 0x2C,
		KC_X           = 0x2D,
		KC_C           = 0x2E,
		KC_V           = 0x2F,
		KC_B           = 0x30,
		KC_N           = 0x31,
		KC_M           = 0x32,
		KC_COMMA       = 0x33,
		KC_PERIOD      = 0x34,    // . on main keyboard
		KC_SLASH       = 0x35,    // / on main keyboard
		KC_RSHIFT      = 0x36,
		KC_MULTIPLY    = 0x37,    // * on numeric keypad
		KC_LMENU       = 0x38,    // left Alt
		KC_SPACE       = 0x39,
		KC_CAPITAL     = 0x3A,
		KC_F1          = 0x3B,
		KC_F2          = 0x3C,
		KC_F3          = 0x3D,
		KC_F4          = 0x3E,
		KC_F5          = 0x3F,
		KC_F6          = 0x40,
		KC_F7          = 0x41,
		KC_F8          = 0x42,
		KC_F9          = 0x43,
		KC_F10         = 0x44,
		KC_NUMLOCK     = 0x45,
		KC_SCROLL      = 0x46,    // Scroll Lock
		KC_NUMPAD7     = 0x47,
		KC_NUMPAD8     = 0x48,
		KC_NUMPAD9     = 0x49,
		KC_SUBTRACT    = 0x4A,    // - on numeric keypad
		KC_NUMPAD4     = 0x4B,
		KC_NUMPAD5     = 0x4C,
		KC_NUMPAD6     = 0x4D,
		KC_ADD         = 0x4E,    // + on numeric keypad
		KC_NUMPAD1     = 0x4F,
		KC_NUMPAD2     = 0x50,
		KC_NUMPAD3     = 0x51,
		KC_NUMPAD0     = 0x52,
		KC_DECIMAL     = 0x53,    // . on numeric keypad
		KC_OEM_102     = 0x56,    // < > | on UK/Germany keyboards
		KC_F11         = 0x57,
		KC_F12         = 0x58,
		KC_F13         = 0x64,    //                     (NEC PC98)
		KC_F14         = 0x65,    //                     (NEC PC98)
		KC_F15         = 0x66,    //                     (NEC PC98)
		KC_KANA        = 0x70,    // (Japanese keyboard)
		KC_ABNT_C1     = 0x73,    // / ? on Portugese (Brazilian) keyboards
		KC_CONVERT     = 0x79,    // (Japanese keyboard)
		KC_NOCONVERT   = 0x7B,    // (Japanese keyboard)
		KC_YEN         = 0x7D,    // (Japanese keyboard)
		KC_ABNT_C2     = 0x7E,    // Numpad . on Portugese (Brazilian) keyboards
		KC_NUMPADEQUALS= 0x8D,    // = on numeric keypad (NEC PC98)
		KC_PREVTRACK   = 0x90,    // Previous Track (KC_CIRCUMFLEX on Japanese keyboard)
		KC_AT          = 0x91,    //                     (NEC PC98)
		KC_COLON       = 0x92,    //                     (NEC PC98)
		KC_UNDERLINE   = 0x93,    //                     (NEC PC98)
		KC_KANJI       = 0x94,    // (Japanese keyboard)
		KC_STOP        = 0x95,    //                     (NEC PC98)
		KC_AX          = 0x96,    //                     (Japan AX)
		KC_UNLABELED   = 0x97,    //                        (J3100)
		KC_NEXTTRACK   = 0x99,    // Next Track
		KC_NUMPADENTER = 0x9C,    // Enter on numeric keypad
		KC_RCONTROL    = 0x9D,
		KC_MUTE        = 0xA0,    // Mute
		KC_CALCULATOR  = 0xA1,    // Calculator
		KC_PLAYPAUSE   = 0xA2,    // Play / Pause
		KC_MEDIASTOP   = 0xA4,    // Media Stop
		KC_VOLUMEDOWN  = 0xAE,    // Volume -
		KC_VOLUMEUP    = 0xB0,    // Volume +
		KC_WEBHOME     = 0xB2,    // Web home
		KC_NUMPADCOMMA = 0xB3,    // , on numeric keypad (NEC PC98)
		KC_DIVIDE      = 0xB5,    // / on numeric keypad
		KC_SYSRQ       = 0xB7,
		KC_RMENU       = 0xB8,    // right Alt
		KC_PAUSE       = 0xC5,    // Pause
		KC_HOME        = 0xC7,    // Home on arrow keypad
		KC_UP          = 0xC8,    // UpArrow on arrow keypad
		KC_PGUP        = 0xC9,    // PgUp on arrow keypad
		KC_LEFT        = 0xCB,    // LeftArrow on arrow keypad
		KC_RIGHT       = 0xCD,    // RightArrow on arrow keypad
		KC_END         = 0xCF,    // End on arrow keypad
		KC_DOWN        = 0xD0,    // DownArrow on arrow keypad
		KC_PGDOWN      = 0xD1,    // PgDn on arrow keypad
		KC_INSERT      = 0xD2,    // Insert on arrow keypad
		KC_DELETE      = 0xD3,    // Delete on arrow keypad
		KC_LWIN        = 0xDB,    // Left Windows key
		KC_RWIN        = 0xDC,    // Right Windows key
		KC_APPS        = 0xDD,    // AppMenu key
		KC_POWER       = 0xDE,    // System Power
		KC_SLEEP       = 0xDF,    // System Sleep
		KC_WAKE        = 0xE3,    // System Wake
		KC_WEBSEARCH   = 0xE5,    // Web Search
		KC_WEBFAVORITES= 0xE6,    // Web Favorites
		KC_WEBREFRESH  = 0xE7,    // Web Refresh
		KC_WEBSTOP     = 0xE8,    // Web Stop
		KC_WEBFORWARD  = 0xE9,    // Web Forward
		KC_WEBBACK     = 0xEA,    // Web Back
		KC_MYCOMPUTER  = 0xEB,    // My Computer
		KC_MAIL        = 0xEC,    // Mail
		KC_MEDIASELECT = 0xED,    // Media Select
		MAX_KC_NUM,
	};

	enum MouseButtonID
	{
		MB_Left = 0, MB_Right, MB_Middle,
		MB_Button3, MB_Button4,	MB_Button5, MB_Button6,	MB_Button7,
		MAX_MB_NUM,
	};

	/// @brief
	/// 输入事件处理器接口
	class IInputEventHandler
	{
	public:
		virtual ~IInputEventHandler() {}

		virtual void OnKeyPressed(KeyCode key) = 0;
		virtual void OnKeyReleased(KeyCode key) = 0;

		virtual void OnMouseMoved(int x, int y, int rel_x, int rel_y) = 0;
		virtual void OnMouseButtonPressed(MouseButtonID id) = 0;
		virtual void OnMouseButtonReleased(MouseButtonID id) = 0;

		virtual void OnJoyStickButtonPressed(int button) = 0;
		virtual void OnJoyStickButtonReleased(int button) = 0;
	};

	/// @brief
	/// 输入事件接受器模板
	/// @par
	///		使用这个模板将任何类的输入响应函数转换为输入系统可接受的对象
	template <typename T>
	class InputEventHandler : public IInputEventHandler
	{
		typedef void(T::*KeyHandlerFunc)(KeyCode key);
		typedef void(T::*MouseMoveHandlerFunc)(int, int, int, int);
		typedef void(T::*MouseButtonHandlerFunc)(MouseButtonID id);
		typedef void(T::*JoyStickButtonHandlerFunc)(int button);
	public:
		InputEventHandler(T* recieverObj,
						  KeyHandlerFunc keyDown = 0,
						  KeyHandlerFunc keyUp = 0,
						  MouseMoveHandlerFunc mouseMove = 0,
						  MouseButtonHandlerFunc mouseButtonDown = 0,
						  MouseButtonHandlerFunc mouseButtonUp = 0
						  )
			: m_Object(recieverObj),
			  m_KeyDownFunc(keyDown),
			  m_KeyUpFunc(keyUp),
			  m_MouseMoveFunc(mouseMove),
			  m_MouseButtonDownFunc(mouseButtonDown),
			  m_MouseButtonUpFunc(mouseButtonUp)

		{
		}

		void OnKeyPressed(KeyCode key)
		{
			if (m_KeyDownFunc) (m_Object->*m_KeyDownFunc)(key);
		}

		void OnKeyReleased(KeyCode key)
		{
			if (m_KeyUpFunc) (m_Object->*m_KeyUpFunc)(key);
		}

		void OnMouseMoved(int x, int y, int rel_x, int rel_y)
		{
			if (m_MouseMoveFunc) (m_Object->*m_MouseMoveFunc)(x, y, rel_x, rel_y);
		}

		void OnMouseButtonPressed(MouseButtonID id)
		{
			if (m_MouseButtonDownFunc) (m_Object->*m_MouseButtonDownFunc)(id);
		}

		void OnMouseButtonReleased(MouseButtonID id)
		{
			if (m_MouseButtonUpFunc) (m_Object->*m_MouseButtonUpFunc)(id);
		}

		void OnJoyStickButtonPressed(int button)
		{
		}

		void OnJoyStickButtonReleased(int button)
		{
		}

	private:
		T*						m_Object;			///< 事件接受对象
		KeyHandlerFunc			m_KeyDownFunc;
		KeyHandlerFunc			m_KeyUpFunc;
		MouseMoveHandlerFunc	m_MouseMoveFunc;
		MouseButtonHandlerFunc	m_MouseButtonDownFunc;
		MouseButtonHandlerFunc	m_MouseButtonUpFunc;
	};

	class IInputSystem : public IPlugin
	{
	public:
		PluginType GetPluginType() const { return PLUGIN_TYPE_INPUT_SYSTEM; }

		/// 初始化输入系统
		virtual void Initialize(RenderWindowHandle rw) = 0;

		/// 关闭输入系统
		virtual void Shutdown() = 0;

		/// 捕获输入设备当前状态
		virtual void CaptureDevice() = 0;

		virtual void ResizeWindow(unsigned int width, unsigned int height) = 0;

		/// 注册输入事件处理对象
		virtual void SetEventHandler(IInputEventHandler* handler) = 0;

		virtual bool GetKeyDown(KeyCode key) = 0;

		virtual bool GetMouseButtonDown(MouseButtonID ButtonCode) = 0;
		virtual int GetMousePosX() = 0;
		virtual int GetMousePosY() = 0;

		virtual int GetJoyStickAbs(int axis) = 0;
	};

	//class Input : public Singleton<Input>
	//{
	//	friend class Singleton<Input>;
	//public:

	//	void Initialize();
	//	void Shutdown();

	//	void CaptureDevice();

	//	void ResizeWindow();

	//	// 键盘状态
	//	bool GetKeyDown(KeyCode key) const { return m_KeyState[key]; }
	//	void SetKeyDown(KeyCode key, bool val) { m_KeyState[key] = val; }

	//	// 鼠标按钮状态
	//	bool GetMouseButtonDown(MouseButtonID ButtonCode) const { return m_MouseButtonState[ButtonCode]; }
	//	void SetMouseButtonDown(MouseButtonID ButtonCode, bool val) { m_MouseButtonState[ButtonCode] = val; }

	//	// 控制杆按键状态
	//	bool GetJoyStickButtonDown(int button) const { return m_JoyStickButton[button]; }
	//	void SetJoyStickButtonDown(int button, bool val) { m_JoyStickButton[button] = val; }

	//	// 控制杆摇杆坐标
	//	int GetJoyStickAbs(int axis) const { return m_JoyStickAxis[axis]; }
	//	void SetJoyStickAbs(int axis, int val) { m_JoyStickAxis[axis] = val; }


	//	float GetJoyStickDeadZone() const { return m_DeadZone; }
	//	void SetJoyStickDeadZone(float deadZone) { m_DeadZone = deadZone; }

	//	int GetMouseRelX() { return m_MouseRelX; }
	//	void SetMouseRelX(int val) { m_MouseRelX = val; }
	//	int GetMouseRelY() { return m_MouseRelY; }
	//	void SetMouseRelY(int val) { m_MouseRelY = val; }

	//	int GetMouseAbsX() { return m_MouseAbsX; }
	//	void SetMouseAbsX(int val) { m_MouseAbsX = val; }
	//	int GetMouseAbsY() { return m_MouseAbsY; }
	//	void SetMouseAbsY(int val) { m_MouseAbsY = val; }

	//protected:
	//	Input();

	//	bool				m_Initialized;

	//	OIS::InputManager*	m_InputManager;
	//	OIS::Keyboard*		m_Keyboard;
	//	OIS::Mouse*			m_Mouse;
	//	OIS::JoyStick*		m_JoyStick;

	//	InputHandler		m_Handler;
	//	bool				m_KeyState[MAX_KC_NUM];
	//	bool				m_MouseButtonState[MAX_MB_NUM];
	//	bool				m_JoyStickButton[24];

	//	int					m_JoyStickAxis[6];
	//	float				m_DeadZone;

	//	int					m_MouseRelX, m_MouseRelY;
	//	int					m_MouseAbsX, m_MouseAbsY;
	//};
}

#endif
