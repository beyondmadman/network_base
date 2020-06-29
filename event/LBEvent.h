#ifndef _LBEVENT_H_
#define _LBEVENT_H_

#include"LBCommon.h"

typedef struct
{
	SOCKET sockfd;
	char buffer[BUFLEN];
	int len;
}event_base;

#ifdef WIN32

#define ACCEPT     1
#define RECV       2
#define SEND       3

typedef struct
{
	OVERLAPPED overlapped;
	SOCKET sockfd;
	char buffer[BUFLEN];
	int len;
	int type;
} event;

typedef union IOCP_DATA
{
	void *ptr;
	SOCKET fd;
} iocp_data;

typedef struct
{
	iocp_data data;
	event* event;
	unsigned long  bytesTrans;
} iocp_event;

#endif

#endif

