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

//��Ϣ��־
#define LOG_TRACE(...)  CSLog::Append("TRACE", __VA_ARGS__);

//������־
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

//��Ϣ��־
#define LOG_INFO(...)  CSLog::Append("INFO", __VA_ARGS__);

//������־
#define LOG_WARN(...)  CSLog::Append("WARN", __VA_ARGS__);

//������־
#define LOG_ERROR(...)  CSLog::Append("ERROR", __VA_ARGS__);

//������Ϣ
#define LOG_FATAL(...)  CSLog::Append("FATA", __VA_ARGS__);

/**
*   ��־
**/
///////////////////////////////////////////////////////////////////////////////////

class CSLog
{
private:
	//���캯��
	CSLog();
	//��������
	~CSLog();

public:
	//��ʼ��
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

	//�����־
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
	
	FILE*                               m_File;                           //��־�ļ�
	mutex                               m_mutex;                          //�������
	LOGTARGET                           m_outTarget;                      //���λ��
};

///////////////////////////////////////////////////////////////////////////////////

#endif // !_CS_LOGFILE_H_

