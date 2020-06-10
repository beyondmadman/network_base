#ifndef _CS_TIME_H_
#define _CS_TIME_H_

#include <time.h>
#include <chrono>
#include <string>

using namespace std::chrono;
using std::string;

static const int64_t kMicroSecondsPerSecond = 1000000000;

/**
*   时间类
**/
///////////////////////////////////////////////////////////////////////////////////

class CCSTime
{
public:
	CCSTime()
		:CCSTime(0)
	{

	}
	CCSTime(int64_t usTimestamp)
		: m_usTimestamp(usTimestamp)
	{

	}

	//获取当前时间
	static CCSTime GetNow()
	{
		auto tp = system_clock::now();
		return CCSTime(system_clock::to_time_t(tp));
	}

	//时间转字符串
	string toString()
	{
		char buf[32] = { 0 };
		int64_t seconds = m_usTimestamp / kMicroSecondsPerSecond;
		int64_t microseconds = m_usTimestamp % kMicroSecondsPerSecond;
		snprintf(buf, sizeof(buf), "%lld.%06lld", seconds, microseconds);
		return buf;
	}

	//时间格式化
	string toFormatString()
	{
		char buf[32] = { 0 };
		tm *tm_time = localtime(&m_usTimestamp);

		snprintf(buf, sizeof(buf), "%4d-%02d-%02d %02d:%02d:%02d",
			tm_time->tm_year + 1900, tm_time->tm_mon + 1, tm_time->tm_mday,
			tm_time->tm_hour, tm_time->tm_min, tm_time->tm_sec);
		return buf;
	}

private:
	int64_t                                  m_usTimestamp;
};

///////////////////////////////////////////////////////////////////////////////////

/**
*   时间戳类
**/
class CCSTimestamp
{
public:
	
	//构造函数
	CCSTimestamp()
	{
		GetNow();
	}

	//析构函数
	~CCSTimestamp()
	{

	}

	void GetNow()
	{
		m_timestamp = high_resolution_clock::now();
	}
	
	//获取当前秒
	double GetElapsedSecond()
	{
		return  GetElapsedTimeInMicroSec() * 0.000001;
	}
	
	//获取毫秒
	double GetElapsedTimeInMilliSec()
	{
		return this->GetElapsedTimeInMicroSec() * 0.001;
	}
	
	//获取微妙
	long long GetElapsedTimeInMicroSec()
	{
		return duration_cast<microseconds>(high_resolution_clock::now() - m_timestamp).count();
	}

private:
	time_point<high_resolution_clock>                         m_timestamp;
};

#endif // !_CS_TIME_H_
