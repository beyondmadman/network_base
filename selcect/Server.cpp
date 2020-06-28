#include "CSFDSet.h"

#define CLIENT_SIZE 64

int GetError()
{
#ifdef WIN32
    return (WSAGetLastError());
#else
    return (errno);
#endif
}

int Close(SOCKET sock)
{
#ifdef WIN32
    return (closesocket(sock));
#else
    return (close(sock));
#endif
}

#define SERV_PORT 8080

int main(int argc, char* argv[])
{

    int ret;

#ifdef _WIN32
    //��ʼ��socket����
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif

    //�����׽���
    SOCKET listenSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listenSock == INVALID_SOCKET)
    {
        printf("socket creation failed.\n");
    }
    else
    {
        printf("socket creation succeeded.\n");
    }

    //���׽���
    sockaddr_in svraddr;
    svraddr.sin_family = AF_INET;
    svraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    svraddr.sin_port = htons(SERV_PORT);
    ret = bind(listenSock, (struct sockaddr*)&svraddr, sizeof(svraddr));
    if (ret == SOCKET_ERROR)
    {
        printf("port binding failed.\n");
    }
    else
    {
        printf("port binding succeeded.\n");
    }

    //�����׽���
    if (SOCKET_ERROR == listen(listenSock, 5))
    {
        printf("port listening failed.\n");
    }
    else
    {
        printf("port listening succeeded\n");
    }

    SOCKET connSock, maxSock, clients[CLIENT_SIZE];
	CSFDSet fdReads,fdTemps;

    //��ʼ���ͻ�������
    for (int i = 0; i < FD_SETSIZE; i++)
    {
        clients[i] = INVALID_SOCKET;
    }
    int clientsIndex = 0;

    maxSock = listenSock;
	fdReads.FD_Zero();
	fdReads.FD_Set(listenSock);

    while (true)
    {
		fdTemps.FD_Copy(fdReads);
        ret = select(maxSock + 1, fdTemps.GetFDSet(), NULL, NULL, NULL);
        if (ret < 0)
        {
            printf("select error.\n");
        }

        //�����¿ͻ�������
        if (fdTemps.FD_IsSet(listenSock))
        {
            sockaddr_in clientAddr;
            socklen_t addrLen = sizeof(sockaddr_in);
            connSock = accept(listenSock, (sockaddr *)&clientAddr, &addrLen);
            if (INVALID_SOCKET == connSock)
            {
                printf("accpet error.\n");
            } 

            printf("client join:socket = %d\n", connSock);
            for (int i = 0; i < CLIENT_SIZE; i++)
            {
                if (clients[i] == INVALID_SOCKET)
                {                           
                    clients[i] = connSock;
                    clientsIndex = (clientsIndex < i) ? i : clientsIndex;
                    break;
                }
            }

			fdReads.FD_Set(connSock);
            maxSock = (maxSock < connSock) ? connSock : maxSock;
        }

        //�������пͻ�������
        SOCKET sock;
        for (int i = 0; i <= clientsIndex; i++)
        {
            sock = clients[i];
            if (sock == INVALID_SOCKET) continue;

            if (fdTemps.FD_IsSet(sock))
            {
                char buf[1024] = {0};
                int len = recv(sock, buf, sizeof(buf), 0);
                //�ͻ��˶Ͽ�
                if (len <= 0)
                {
                    Close(sock);
					fdReads.FD_Clr(sock);
                    clients[i] = INVALID_SOCKET;
                    printf("client exit:socket = %d\n", sock);
                }	
                //������Ϣ
                else
                {
                    printf("recv %s\n",buf);

                    for (int j = 0; j < len; j++)
                    {
                        buf[j] = toupper(buf[j]);
                    }

                    printf("toupper %s\n",buf);
                    send(sock, buf, strlen(buf)+1, 0);
                }
            }
        }
    }

    Close(listenSock);

    //ж��socket����
#ifdef _WIN32
    WSACleanup();	
#endif

    getchar();
    return 0;
}
