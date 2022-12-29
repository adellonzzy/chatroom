#include "sockcreate.h"
int Sockfd_Create()
{
    int sockfd = socket(__domain,__type,__protocol);
    if(-1 == sockfd)
    {
        perror("socket error");
		return -1;
    }
    return sockfd;
}

int Bind_ipv4(int sockfd,const char *IP,const char *Port)
{
    struct sockaddr_in  serveraddr;
    serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(IP);
	serveraddr.sin_port = htons(atoi(Port));
    socklen_t addrlen = sizeof(serveraddr);

	// int opt = 1;
    // setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));  //设置地址可以被重复绑定

	if(bind(sockfd,(struct sockaddr *)&serveraddr,addrlen) == -1)
	{
		perror("bind");
		return -1;
	}
    return 0;
}

int Listen_Client(int sockfd,unsigned int Client_Num)
{
	if(listen(sockfd,Client_Num) == -1)
	{
		perror("listen");
		return -1;
	}
    return 0;
}


//int Accept_Client(int sockfd)
void *Accept_Client(int sockfd)
{
	Client_INFO *client_info = (Client_INFO *)malloc(sizeof(Client_INFO));
	//printf("%p\n",client_info);
    //struct sockaddr_in  clientaddr;
	socklen_t addrlen = sizeof(client_info->clientaddr);
	//int fd = accept(sockfd,(struct sockaddr *)&clientaddr,&addrlen);
	client_info->fd = accept(sockfd,(struct sockaddr *)&client_info->clientaddr,&addrlen);
    if(client_info->fd == -1)
	{
		ERRLOG("accept");
	}
    printf("客户端%s:%d连接了\n",inet_ntoa(client_info->clientaddr.sin_addr),ntohs(client_info->clientaddr.sin_port));

    return client_info;
}

int Connect_Server(int sockfd,const char *IP,const char *Port)
{
    struct sockaddr_in  serveraddr;
    serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(IP);
	serveraddr.sin_port = htons(atoi(Port));
    socklen_t addrlen = sizeof(serveraddr);

	if(-1 == connect(sockfd,(struct sockaddr *)&serveraddr,addrlen))
	{
		perror("connect");	
		return -1;
	}
    return 0;
}
#if 0
//send发送
void mysend(int fd,void *content)
{
	;
}
//recv接收
void myrecv(int fd,void *content)
{
	;
}
#endif