#ifndef _LBIOCP_H_
#define _LBIOCP_H_

#ifdef WIN32

#include "LBEvent.h"

#include <mswsock.h>
#pragma comment(lib,"Mswsock.lib")

class LBIOCP
{
public:
	LBIOCP()
	{
		events = new iocp_event;
	}

	~LBIOCP()
	{
		Destory();
	}
	void Init()
	{
		//´´½¨IOCP
		m_hCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0);
		if (m_hCompletionPort == NULL)
		{
			printf("CreateIoCompletionPort error.\n");
		}
	}

	void Destory()
	{
		if (NULL != m_hCompletionPort)
		{
			CloseHandle(m_hCompletionPort);
			m_hCompletionPort = NULL;
		}
	}

	bool AddEvent(SOCKET sockfd, void *ptr = NULL)
	{
		ULONG_PTR pCompleteKey = (ptr == NULL) ? (ULONG_PTR)sockfd : (ULONG_PTR)ptr;
		HANDLE hRet = CreateIoCompletionPort((HANDLE)sockfd, m_hCompletionPort, (ULONG_PTR)sockfd, 0);
		if (hRet == NULL)
		{
			printf("CreateIoCompletionPort error.\n");
			return false;
		}
		return true;
	}

	void PostAccept(event *ev)
	{
		ev->sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		ev->type = ACCEPT;
		BOOL bRet = AcceptEx(
			m_lsockfd,
			ev->sockfd,
			ev->buffer,
			0,
			sizeof(sockaddr_in) + 16,
			sizeof(sockaddr_in) + 16,
			NULL,
			&ev->overlapped);
		if (FALSE == bRet)
		{
			if (ERROR_IO_PENDING != GetLastError())
			{
				printf("AcceptEx error.\n");
			}
		}
	}

	void PostRecv(event *ev)
	{
		ev->type = RECV;
		WSABUF wsaBuf = {};
		wsaBuf.buf = ev->buffer;
		wsaBuf.len = BUFLEN;

		ZeroMemory(&ev->overlapped, sizeof(OVERLAPPED));

		DWORD dwFlag = 0;
		int nRet = WSARecv(
			ev->sockfd,
			&wsaBuf,
			1,
			NULL,
			&dwFlag,
			&ev->overlapped,
			NULL);
		if (nRet == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSA_IO_PENDING)
			{
				printf("WSARecv error.\n");
			}
		}

	}

	void PostSend(event *ev)
	{
		ev->type = SEND;
		WSABUF wsaBuf = {};
		wsaBuf.buf = ev->buffer;
		wsaBuf.len = ev->len;

		ZeroMemory(&ev->overlapped, sizeof(OVERLAPPED));

		DWORD dwFlag = 0;
		int nRet = WSASend(
			ev->sockfd,
			&wsaBuf,
			1,
			NULL,
			dwFlag,
			&ev->overlapped,
			NULL);
		if (nRet == SOCKET_ERROR)
		{
			int erro = WSAGetLastError();
			if (erro != WSA_IO_PENDING)
			{
				printf("WSASend error.%d\n", erro);
			}
		}

	}

	int ProcessEvents(unsigned int time)
	{
		events->bytesTrans = 0;
		events->event = NULL;
		events->data.ptr = NULL;

		if (FALSE == GetQueuedCompletionStatus(m_hCompletionPort,
			&events->bytesTrans,
			(PULONG_PTR)&events->data,
			(LPOVERLAPPED*)&events->event,
			time))
		{
			DWORD dwError = GetLastError();
			if (WAIT_TIMEOUT == dwError)
			{
				return 0;
			}
			if (ERROR_NETNAME_DELETED == dwError)
			{
				return 1;
			}
			if (ERROR_CONNECTION_ABORTED == dwError)
			{
				return 1;
			}
			if (ERROR_SEM_TIMEOUT == dwError)
			{
				return 1;
			}
			printf("GetQueuedCompletionStatus falied.\n");
			return -1;
		}
		return 1;
	}

	iocp_event *getEvents()
	{
		return events;
	}


	bool loadAcceptEx(SOCKET lSockfd)
	{
		GUID GuidAcceptEx = WSAID_ACCEPTEX;
		DWORD dwBytes = 0;
		int Ret = WSAIoctl(lSockfd, SIO_GET_EXTENSION_FUNCTION_POINTER,
			&GuidAcceptEx, sizeof(GuidAcceptEx),
			&m_AccecptEx, sizeof(m_AccecptEx),
			&dwBytes, NULL, NULL);

		if (SOCKET_ERROR == Ret)
		{
			printf("WSAIoctl error.\n");
			return false;
		}

		m_lsockfd = lSockfd;

		return true;
	}

private:
	HANDLE		m_hCompletionPort = NULL;
	SOCKET		m_lsockfd;
	iocp_event* events;
	LPFN_ACCEPTEX m_AccecptEx;
};

#endif // WIN32

#endif

