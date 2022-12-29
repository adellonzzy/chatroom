#ifndef _SOCKCREATE_H_
#define _SOCKCREATE_H_
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <sys/time.h>
#include <signal.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#define MAXSIZE 1024
#define MAX_Client 100
#define __domain AF_INET
#define __type SOCK_STREAM
#define __protocol 0
//设置服务器地址以及端口号
// #define IP_addr "192.168.59.163"
// #define Port_num "8888"

#define ERRLOG(errmsg) do{\
							perror(errmsg);\
							printf("%s - %s - %d\n",__FILE__, __func__,__LINE__);\
							return NULL;\
						 }while(0)
						 
//客户端信息
typedef struct Client_INFO
{
	int fd;
	struct sockaddr_in clientaddr;
}Client_INFO;

//创建套接字
int Sockfd_Create();
//ipv4通信方式绑定套接字
int Bind_ipv4(int sockfd,const char *IP,const char *Port);
//服务器设置监听
int Listen_Client(int sockfd,unsigned int Client_Num);
//服务器等待客户端连接
//int Accept_Client(int sockfd);
void *Accept_Client(int sockfd);
//客户端发起连接请求
int Connect_Server(int sockfd,const char *IP,const char *Port);
//send发送
void mysend(int fd,void *content);
//recv接收
void myrecv(int fd,void *content);

#endif