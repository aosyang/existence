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
#include "EString.h"

using namespace std;

// 屏蔽警告
#pragma warning(push)
#pragma warning(disable:4996)

#include "Singleton.h"

class Log : public Singleton<Log>
{
	friend class Singleton<Log>;
public:

	~Log()
	{
		logfile.close();
	}

	void CreateLog(const String& filename)
	{
		if (logfile.is_open())
			logfile.close();

		logfile.open(filename.Data());

		assert(logfile.is_open());
	}

	void Msg(const String& msg)
	{
		logfile << msg.Data();
		cout << msg.Data();
	}

	void MsgLn(const String& msg)
	{
		OutputTime();
		logfile << msg.Data() << endl;
		cout << msg.Data() << endl;

		//logfile.flush();
	}

	// 输出时间
	void OutputTime()
	{
		time_t curtime = time(0); 
		tm t = *localtime(&curtime);

		String msg;
		msg.Format("%02d:%02d:%02d - ", t.tm_hour, t.tm_min, t.tm_sec);

		logfile << msg.Data();
		cout << msg.Data();
	}

	// 警告消息
	void Warning(const String& msg)
	{
		OutputTime();

		logfile << "!! Warning !! : ";
		cout << "!! Warning !! : ";

		logfile << msg.Data() << endl;
		cout << msg.Data() << endl;

		//logfile.flush();
	}

	// 错误消息
	void Error(const String& msg)
	{
		OutputTime();

		logfile << ">> ERROR << : ";
		cout << ">> ERROR << : ";

		logfile << msg.Data() << endl;
		cout << msg.Data() << endl;

		//logfile.flush();
	}


#define LOGREGTYPE(type) \
	Log& operator << (type _Val) \
	{ \
		cout << _Val; \
		logfile << _Val; \
		return *this; \
	} \

	LOGREGTYPE(bool)
	LOGREGTYPE(int)
	LOGREGTYPE(unsigned int)
	LOGREGTYPE(long)
	LOGREGTYPE(unsigned long)
	LOGREGTYPE(float)
	LOGREGTYPE(double)
	LOGREGTYPE(const string&)
	LOGREGTYPE(const char*)

	Log& operator<<(const String& rhs)
	{
		cout << rhs.Data();
		logfile << rhs.Data();
		return *this;
	}


private:
	Log() {}

	ofstream  logfile;
};

#pragma warning(pop)

#define Log Log::Instance()


#endif
