//-----------------------------------------------------------------------------------
/// Log.h 日志，记录信息到文件，并输出到控制台(如果有控制台)
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



// 屏蔽警告(for win32 only)
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
		///	向日志管理器中增加一个日志接收器
		/// @param reciever
		///		接收器指针
		/// @remarks
		///		添加后的接收器将在引擎销毁时一同销毁，如果需要手动销毁请及时从管理器中移除
		void AddReciever(ILogReciever* reciever);

		/// @brief
		/// 移除日志接收器
		/// @param reciever
		///		要移除的日志接收器
		void RemoveReciever(ILogReciever* reciever);

		void Msg(const String& msg);

		void MsgLn(const String& msg);

		// 输出时间
		void OutputTime();

		// 警告消息
		void Warning(const String& msg);

		// 错误消息
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

		std::set<ILogReciever*>		m_LogRecievers;		///< 日志接收器列表
	};

	/// @brief
	///	日志接收器接口
	/// @par
	///		用户的日志接受对象从这个接口派生，并在日志管理器中注册，便可接受到引擎发送的日志
	class ILogReciever
	{
	public:
		/// 虚析构函数
		virtual ~ILogReciever() {}

		/// @brief
		///	输出日志接口
		/// @par
		///		覆写这个方法可以将日志输出到不同的对象中去
		/// @param rhs
		///		要写入的日志内容
		virtual void OutputLog(const String& rhs) = 0;
	};

	/// @brief
	///	文件日志接收器
	/// @par
	///		向文件中写入日志内容的接收器
	class IOLogReciever : public ILogReciever
	{
	public:
		~IOLogReciever();

		/// @brief
		///	打开文件作为日志输出
		/// @param filename
		///		输出日志的文件名
		/// @remarks
		///		如果未能正确打开文件，该方法将触发assert
		void OpenFileForOutput(const String& filename);

		/// @brief
		/// 输出日志
		/// @copydoc ILogReciever::OutputLog(const String&)
		void OutputLog(const String& rhs);

	private:
		std::ofstream	m_LogFile;			///< 日志文件输出流
	};

#if defined __PLATFORM_WIN32
#pragma warning(pop)
#endif	//#if defined __PLATFORM_WIN32

#define Log LogManager::Instance()
}

#endif
