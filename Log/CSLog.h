#ifndef _CS_LOGFILE_H_
#define _CS_LOGFILE_H_

#include <string>
#include <stdio.h>
#include <mutex>
#include <stdarg.h>

#include "CSTime.h"

using std::string;
using std::mutex;

enum LOGTARGET
{
	LOGTARGET_CONSOLE,
	LOGTARGET_FILE,
	LOGTARGET_NONE
};

//#define LOG_INIT() CSLog::Instance();

//信息日志
#define LOG_TRACE(...)  CSLog::Append("TRACE", __VA_ARGS__);

//调试日志
#define FILENAME(x) strrchr(x,'\\')?strrchr(x,'\\')+1:x
#ifdef _DEBUG
	#ifndef LOG_DEBUG
		#define LOG_DEBUG(...) CSLog::Append("DEBUG",(unsigned char*)(FILENAME(__FILE__)),(unsigned char*)(__FUNCTION__), (int)(__LINE__), __VA_ARGS__);
	#endif 
#else
	#ifndef LOG_DEBUG
		#define LOG_DEBUG(...)
	#endif 
#endif 

//信息日志
#define LOG_INFO(...)  CSLog::Append("INFO", __VA_ARGS__);

//警告日志
#define LOG_WARN(...)  CSLog::Append("WARN", __VA_ARGS__);

//错误日志
#define LOG_ERROR(...)  CSLog::Append("ERROR", __VA_ARGS__);

//致命信息
#define LOG_FATAL(...)  CSLog::Append("FATA", __VA_ARGS__);

/**
*   日志
**/
///////////////////////////////////////////////////////////////////////////////////

class CSLog
{
private:
	//构造函数
	CSLog();
	//析构函数
	~CSLog();

public:
	//初始化
	void InitLog(LOGTARGET logTarget,const string &fileName = "")
	{
		if (logTarget == LOGTARGET_FILE)
		{
			m_File = fopen(fileName.c_str(), "w");
			if (m_File)
			{
				printf("InitLog successe");
			}
			else
			{
				printf("InitLog failed");
			}
		}

		m_outTarget = logTarget;
	}

	static CSLog& Instance()
	{
		static  CSLog log;
		return log;
	}

	//添加日志
    template<typename ...Args>
	static void Append(const char* logLevel,
		unsigned char* fileName,
		unsigned char* function,
		int lineNumber,  
		char* format,
		Args ... args)
	{
		CSLog* pLog = &Instance();

		std::lock_guard<mutex> lock(pLog->m_mutex);

		if(pLog->m_outTarget == LOGTARGET_FILE) 
		{
			fprintf(pLog->m_File, "[%s] %s: ", CCSTime::GetNow().toFormatString().c_str(), logLevel);
			fprintf(pLog->m_File, format, args...);
			fprintf(pLog->m_File, "\n");
		}
		else if (pLog->m_outTarget == LOGTARGET_CONSOLE)
		{
			printf("[%s] %s: flie:%s fun:%s line:%d ", CCSTime::GetNow().toFormatString().c_str(), logLevel, fileName, function, lineNumber);
			printf(format, args...);
			printf("\n");
		}
	}
	
	FILE*                               m_File;                           //日志文件
	mutex                               m_mutex;                          //互斥对象
	LOGTARGET                           m_outTarget;                      //输出位置
};

///////////////////////////////////////////////////////////////////////////////////

#endif // !_CS_LOGFILE_H_

