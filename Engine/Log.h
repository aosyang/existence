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
#include <string>

using namespace std;

// ���ξ���
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

	void CreateLog(const char* filename)
	{
		if (logfile.is_open())
			logfile.close();

		logfile.open(filename);

		assert(logfile.is_open());
	}

	void Msg(const char* msg)
	{
		logfile << msg;
		cout << msg;
	}

	void MsgLn(const char* msg)
	{
		OutputTime();
		logfile << msg << endl;
		cout << msg << endl;

		//logfile.flush();
	}

	// ���ʱ��
	void OutputTime()
	{
		time_t curtime = time(0); 
		tm t = *localtime(&curtime);

		char buf[20];
		sprintf(buf, "%02d:%02d:%02d - ", t.tm_hour, t.tm_min, t.tm_sec);

		logfile << buf;
		cout << buf;
	}

	// ������Ϣ
	void Warning(const char* msg)
	{
		OutputTime();

		logfile << "!! Warning !! : ";
		cout << "!! Warning !! : ";

		logfile << msg << endl;
		cout << msg << endl;

		//logfile.flush();
	}

	// ������Ϣ
	void Error(const char* msg)
	{
		OutputTime();

		logfile << ">> ERROR << : ";
		cout << ">> ERROR << : ";

		logfile << msg << endl;
		cout << msg << endl;

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


private:
	Log() {}

	ofstream  logfile;
};

#pragma warning(pop)

#define Log Log::Instance()


#endif
