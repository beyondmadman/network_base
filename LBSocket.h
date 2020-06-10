#ifndef _LBSOCKET_H_
#define _LBSOCKET_H_

#ifdef _WIN32
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include<WinSock2.h>
#pragma comment(lib,"ws2_32.lib")

typedef int	socklen_t;
#else
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<ctype.h>
#include<string.h>
#include<errno.h>

typedef int SOCKET;
#define INVALID_SOCKET  (SOCKET)(~0)
#define SOCKET_ERROR            (-1)

#endif

#include<stdio.h>

class LBSocket
{
public:
	//构造函数
	LBSocket()
	{
		m_sockfd = INVALID_SOCKET;
	}

	//析构函数
	~LBSocket()
	{
	}

	//初始网络环境
	static int LBSocket::Init()
	{
#ifdef WIN32
		WSADATA wsaData;
		int ret = WSAStartup(MAKEWORD(2, 0), &wsaData);
		if (ret)
		{
			printf("Initilize winsock error\n");
			return -1;
		}
#endif

		return 0;
	}

	//注销网络环境
	static int Clean()
	{
#ifdef WIN32
		return (WSACleanup());
#endif
		return 0;
	}

	//创建套接字
	SOCKET Create(int af, int type, int protocol)
	{
		m_sockfd = socket(af, type, protocol);
		if (m_sockfd == INVALID_SOCKET)
		{
			printf("create socket failed,errcode = %d\n", GetError());
		}
		return m_sockfd;
	}

	//连接套接字
	bool Connect(const char* ip, unsigned short port)
	{
		struct sockaddr_in svraddr;
		svraddr.sin_family = AF_INET;
		svraddr.sin_addr.s_addr = inet_addr(ip);
		svraddr.sin_port = htons(port);
		int ret = connect(m_sockfd, (struct sockaddr*)&svraddr, sizeof(svraddr));
		if (ret == SOCKET_ERROR)
		{
			printf("server connection failed,errcode = %d\n", GetError());
			return false;
		}
		return true;
	}

	//绑定套接字
	bool Bind(unsigned short port)
	{
		struct sockaddr_in svraddr;
		svraddr.sin_family = AF_INET;
		svraddr.sin_addr.s_addr = INADDR_ANY;
		svraddr.sin_port = htons(port);

		int ret = bind(m_sockfd, (struct sockaddr*)&svraddr, sizeof(svraddr));
		if (ret == SOCKET_ERROR)
		{
			printf("socket bind failed,errcode = %d\n", GetError());
			return false;
		}
		return true;
	}

	//监听套接字
	bool Listen(int backlog)
	{
		int ret = listen(m_sockfd, backlog);
		if (ret == SOCKET_ERROR)
		{
			printf("socket listen failed,errcode = %d\n", GetError());
			return false;
		}
		return true;
	}

	//接收套接字
	SOCKET LBSocket::Accept()
	{
		struct sockaddr_in cliaddr;
		socklen_t addrlen = sizeof(cliaddr);
		SOCKET clientSockfd = INVALID_SOCKET;
		clientSockfd = accept(m_sockfd, (struct sockaddr*)&cliaddr, &addrlen);
		if (clientSockfd == SOCKET_ERROR)
		{
			printf("accept socket failed,errcode = %d\n", GetError());
		}

		return clientSockfd;
	}

	//端口复用
	bool SetReuseAddr(SOCKET sockfd)
	{
		int opt = 1;
		if (SOCKET_ERROR == setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt)))
		{
			return false;
		}
		return true;
	}

	//设置阻塞
	bool setNonBlock(SOCKET sockfd)
	{
#ifndef WIN32
		int flags = 0;
		if ((flags = ::fcntl(sockfd, F_GETFL, 0)) != SOCKET_ERROR)
		{
			flags |= O_NONBLOCK;
			if (::fcntl(sockfd, F_SETFL, flags) != SOCKET_ERROR)
			{
				return true;
			}
		}
		return false;
#else
		return true;
#endif
	}

	//关闭套接字
	int Close(SOCKET sockfd)
	{
#ifdef WIN32
		return (closesocket(sockfd));
#else
		return (close(sockfd));
#endif
	}

	//套接字错误
	int LBSocket::GetError()
	{
#ifdef WIN32
		return (WSAGetLastError());
#else
		return (errno);
#endif
	}

	//获取套接字
	SOCKET fd() const { return m_sockfd; }

private:
	SOCKET                           m_sockfd;
};

#endif

