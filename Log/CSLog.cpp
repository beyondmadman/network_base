#include "CSLog.h"

///////////////////////////////////////////////////////////////////////////////////

//���캯��
CSLog::CSLog()
{
	
}
//��������
CSLog::~CSLog()
{
	if (m_File)
	{
		fclose(m_File);
		m_File = nullptr;
	}
}



