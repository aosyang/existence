#include "Log.h"
#include "Debug.h"

namespace Gen
{
	LogManager::LogManager()
	{
	}

	LogManager::~LogManager()
	{
		std::set<ILogReciever*>::iterator iter;
		for (iter=m_LogRecievers.begin(); iter!=m_LogRecievers.end(); iter++)
		{
			delete (*iter);
		}
	}

	void LogManager::AddReciever(ILogReciever* reciever)
	{
		AssertFatal(m_LogRecievers.find(reciever)==m_LogRecievers.end(),
					"LogManager::AddReciever() : The reciever has been added already.");

		m_LogRecievers.insert(reciever);
	}

	void LogManager::RemoveReciever(ILogReciever* reciever)
	{
		std::set<ILogReciever*>::iterator iter;
		if ((iter=m_LogRecievers.find(reciever))==m_LogRecievers.end())
			return;

		m_LogRecievers.erase(iter);
	}

	void LogManager::Msg(const String& msg)
	{
		(*this) << msg;
	}

	void LogManager::MsgLn(const String& msg)
	{
		OutputTime();
		(*this) << msg << "\n";
	}

	// 输出时间
	void LogManager::OutputTime()
	{
		time_t curtime = time(0); 
		tm t = *localtime(&curtime);

		String msg;
		msg.Format("%02d:%02d:%02d - ", t.tm_hour, t.tm_min, t.tm_sec);

		(*this) << msg;
	}

	// 警告消息
	void LogManager::Warning(const String& msg)
	{
		OutputTime();

		(*this) << "!! Warning !! : " << msg << "\n";
	}

	// 错误消息
	void LogManager::Error(const String& msg)
	{
		OutputTime();

		(*this) << ">> ERROR << : " << msg << "\n";
	}

	LogManager& LogManager::operator<<(const String& rhs)
	{
		std::set<ILogReciever*>::iterator iter;
		for (iter=m_LogRecievers.begin(); iter!=m_LogRecievers.end(); iter++)
		{
			(*iter)->OutputLog(rhs);
		}
		return *this;
	}



	IOLogReciever::~IOLogReciever()
	{
		m_LogFile.close();
	}

	void IOLogReciever::OpenFileForOutput(const String& filename)
	{
		if (m_LogFile.is_open())
			m_LogFile.close();

		m_LogFile.open(filename.Data());

		AssertFatal(m_LogFile.is_open(), "IOLogReciever::OpenFileForOutput() : Unable to open log file");
	}

	void IOLogReciever::OutputLog(const String& rhs)
	{
		m_LogFile << rhs.Data();
	}

}
