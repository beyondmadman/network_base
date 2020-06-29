#ifndef _LBEPOLL_H_
#define _LBEPOLL_H_

#ifdef __linux__

#include"LBCommon.h"
#include <sys/epoll.h>

class EPOOl
{
public:
	EPOOl()
	{
		events = new epoll_event[MAX_EVENTS];
	}

	~EPOOl()
	{
		if (events != NULL)
		{
			delete[] events;
			events = NULL;
		}
		if (efd > 0)
		{
			close(efd);
			efd = SOCKET_ERROR;
		}
	}

	void init()
	{
		efd = epoll_create(MAX_EVENTS);
		if (efd == -SOCKET_ERROR)
		{
			printf("epoll_create error.\n");
		}
	}

	void eventadd(SOCKET sockfd)
	{
		epoll_event epv = { 0,{ 0 } };
		epv.data.fd = sockfd;
		epv.events = EPOLLIN;

		int ret = epoll_ctl(efd, EPOLL_CTL_ADD, epv.data.fd, &epv);
		if (ret == SOCKET_ERROR)
		{
			printf("epoll_ctl error.\n");
		}
	}

	void eventdel(SOCKET sockfd)
	{
		epoll_event epv = { 0,{ 0 } };
		epv.data.fd = sockfd;

		epoll_ctl(efd, EPOLL_CTL_DEL, epv.data.fd, &epv);

	}

	int processEvents(unsigned int time)
	{
		int ret = epoll_wait(efd, events, MAX_EVENTS, -time);
		if (ret == SOCKET_ERROR)
		{
			printf("epoll_wait error.\n");
		}

		return ret;
	}

	epoll_event *getEvents()
	{
		return events;
	}

	SOCKET efd;
	epoll_event *events;
};

#endif // __linux__

#endif

