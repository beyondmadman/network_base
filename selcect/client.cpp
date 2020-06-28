#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include <WinSock2.h>
#include <stdio.h>
#pragma comment(lib,"ws2_32.lib")


#define BUFLEN 4096
#define SERV_PORT   8080

int main(int argc, char *argv[])
{
	//初始化socket环境
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	//创建套接字
	SOCKET sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sockfd == INVALID_SOCKET)
	{
		printf("socket creation failed.\n");
	}
	else
	{
		printf("socket creation succeeded.\n");
	}

	//绑定套接字
	sockaddr_in svraddr;
	svraddr.sin_family = AF_INET;
	svraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	svraddr.sin_port = htons(SERV_PORT);
	int ret;
	ret = connect(sockfd, (sockaddr*)&svraddr, sizeof(svraddr));
	if (ret == SOCKET_ERROR)
	{
		printf("connect failed.\n");
	}
	else
	{
		printf("connect succeeded.\n");
	}


	while (true)
	{
		char cmdbuf[BUFLEN] = {};
		scanf("%s", cmdbuf);
		if (0 == strcmp(cmdbuf, "exit"))
		{
			break;
		}
		else
		{
			int nRet = send(sockfd, cmdbuf, strlen(cmdbuf) + 1, 0);
			if (nRet == SOCKET_ERROR)
			{
				printf("send error.\n");
				return 0;
			}
		}

		char buf[BUFLEN] = {};
		int nRet = recv(sockfd, buf, BUFLEN, 0);
		if (nRet == 0 || nRet == SOCKET_ERROR)
		{
			printf("the other side has been closed.\n");
			return 0;
		}
		else if (nRet > 0)
		{
			printf("%s\n", buf);
		}
	}

	closesocket(sockfd);

	//卸载socket环境
	WSACleanup();

	return 0;
}