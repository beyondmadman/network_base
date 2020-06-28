#ifndef CSFDSET_H
#define CSFDSET_H

#include "CSCommon.h"

//#define FD_SETSIZE     10240
#define FD_MAX_SIZE    10240

class CSFDSet
{
public:

	CSFDSet()
		:m_pfdset(NULL),
		m_nfdSize(0)
	{
		int fdMaxSize = FD_MAX_SIZE;
#ifdef _WIN32
		m_nfdSize = sizeof(u_int) + (sizeof(SOCKET) * fdMaxSize);
#else
		m_nfdSize = fdMaxSize / (8 * sizeof(char));
#endif

		m_pfdset = (fd_set*) new char[m_nfdSize];
		memset(m_pfdset, 0, m_nfdSize);
	}

	~CSFDSet()
	{
		if (m_pfdset)
		{
			delete[] m_pfdset;
			m_pfdset = NULL;
		}
	}

	inline void FD_Set(SOCKET s)
	{
#ifdef _WIN32
		FD_SET(s, m_pfdset);
#else
		if (s < FD_MAX_SIZE)
		{
			FD_SET(s, m_pfdset);
		}
		else
		{
			printf("SOCKET %d FD_MAX_SIZE %d", (int)s, FD_MAX_SIZE);
		}
#endif

	}

	inline void FD_Clr(SOCKET s)
	{
		FD_CLR(s, m_pfdset);
	}

	inline void FD_Zero()
	{
#ifdef _WIN32
		FD_ZERO(m_pfdset);
#else
		memset(m_pfdset, 0, m_nfdSize);
#endif 

	}

	inline int FD_IsSet(SOCKET s)
	{
		return FD_ISSET(s, m_pfdset);
	}

	inline fd_set* GetFDSet()
	{
		return m_pfdset;
	}

	inline void FD_Copy(CSFDSet& set)
	{
		memcpy(m_pfdset, set.m_pfdset, set.m_nfdSize);
	}


private:
	fd_set*			m_pfdset;
	size_t			m_nfdSize;
};

#endif

