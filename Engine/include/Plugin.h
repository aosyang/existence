#ifndef _PLUGIN_H
#define _PLUGIN_H

#include "Platform.h"
#include "EString.h"
#include "IPlugin.h"

namespace Gen
{

#if defined __PLATFORM_WIN32
	typedef HINSTANCE Module_t;

#elif defined __PLATFORM_LINUX
	typedef void* Module_t;

#endif	// #if defined __PLATFORM_WIN32

	/// @brief
	/// ������
	/// @par
	///		ͨ���ӿ��ṩ������뵽���������ⲿ����
	class Plugin
	{
	public:
		Plugin();
		~Plugin();

		bool LoadModule(const String& filename);

		/// @brief
		/// �������ʵ��
		/// @remarks
		///		�����������ȷ��Ŀ������о���CreatePluginInstance��һ����
		IPlugin* CreatePluginInstance();
		//void* GetFunction(const String& name);

		void Unload();

	private:
		Module_t		m_Module;		///< ������
	};

}

#endif
