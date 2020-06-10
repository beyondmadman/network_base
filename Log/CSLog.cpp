#include "CSLog.h"

///////////////////////////////////////////////////////////////////////////////////

//构造函数
CSLog::CSLog()
{
	
}
//析构函数
CSLog::~CSLog()
{
	if (m_File)
	{
		fclose(m_File);
		m_File = nullptr;
	}
}



