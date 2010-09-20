//-----------------------------------------------------------------------------------
/// Log.h ��־����¼��Ϣ���ļ��������������̨(����п���̨)
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------

#ifndef _LOG_H
#define _LOG_H

#include <time.h>
#include <iostream>
#include <fstream>
#include <set>
#include "EString.h"



// ���ξ���(for win32 only)
#if defined __PLATFORM_WIN32
#pragma warning(push)
#pragma warning(disable:4996)
#endif	//#if defined __PLATFORM_WIN32

#include "Singleton.h"

namespace Gen
{
	class ILogReciever;

	class LogManager : public Singleton<LogManager>
	{
		friend class Singleton<LogManager>;
	public:

		~LogManager();

		/// @brief
		///	����־������������һ����־������
		/// @param reciever
		///		������ָ��
		/// @remarks
		///		��Ӻ�Ľ�����������������ʱһͬ���٣������Ҫ�ֶ������뼰ʱ�ӹ��������Ƴ�
		void AddReciever(ILogReciever* reciever);

		/// @brief
		/// �Ƴ���־������
		/// @param reciever
		///		Ҫ�Ƴ�����־������
		void RemoveReciever(ILogReciever* reciever);

		void Msg(const String& msg);

		void MsgLn(const String& msg);

		// ���ʱ��
		void OutputTime();

		// ������Ϣ
		void Warning(const String& msg);

		// ������Ϣ
		void Error(const String& msg);

#define LOGREGTYPE(type) \
		inline LogManager& operator << (type _Val) \
		{ \
			operator<<(String(_Val)); \
			return *this; \
		} \

		LOGREGTYPE(bool)
		LOGREGTYPE(int)
		LOGREGTYPE(unsigned int)
		LOGREGTYPE(long)
		LOGREGTYPE(unsigned long)
		LOGREGTYPE(float)
		LOGREGTYPE(double)
		LOGREGTYPE(const std::string&)
		LOGREGTYPE(const char*)

		LogManager& operator<<(const String& rhs);


	private:
		LogManager();

		std::set<ILogReciever*>		m_LogRecievers;		///< ��־�������б�
	};

	/// @brief
	///	��־�������ӿ�
	/// @par
	///		�û�����־���ܶ��������ӿ�������������־��������ע�ᣬ��ɽ��ܵ����淢�͵���־
	class ILogReciever
	{
	public:
		/// ����������
		virtual ~ILogReciever() {}

		/// @brief
		///	�����־�ӿ�
		/// @par
		///		��д����������Խ���־�������ͬ�Ķ�����ȥ
		/// @param rhs
		///		Ҫд�����־����
		virtual void OutputLog(const String& rhs) = 0;
	};

	/// @brief
	///	�ļ���־������
	/// @par
	///		���ļ���д����־���ݵĽ�����
	class IOLogReciever : public ILogReciever
	{
	public:
		~IOLogReciever();

		/// @brief
		///	���ļ���Ϊ��־���
		/// @param filename
		///		�����־���ļ���
		/// @remarks
		///		���δ����ȷ���ļ����÷���������assert
		void OpenFileForOutput(const String& filename);

		/// @brief
		/// �����־
		/// @copydoc ILogReciever::OutputLog(const String&)
		void OutputLog(const String& rhs);

	private:
		std::ofstream	m_LogFile;			///< ��־�ļ������
	};

#if defined __PLATFORM_WIN32
#pragma warning(pop)
#endif	//#if defined __PLATFORM_WIN32

#define Log LogManager::Instance()
}

#endif
