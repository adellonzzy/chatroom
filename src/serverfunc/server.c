#include "sockcreate.h"
#include "myserver.h"
#include "sqlite_op.h"
//#define IP_addr "192.168.59.176"
#define IP_addr "127.0.0.1"
#define Port_num "8888"

// pthread_mutex_t mutex;
// pthread_cond_t cond;

int main(int argc, char const *argv[])
{
    int sockfd = Sockfd_Create();
    int acceptfd;
    char IP[] = IP_addr;
    char Port[] = Port_num;
    Bind_ipv4(sockfd,IP,Port);
    Listen_Client(sockfd,MAX_Client);

    //消息收发
    char buf[N] = {0};
    //读取字节数
    ssize_t bytes;

    // struct sockaddr_in  clientaddr;
    // socklen_t addrlen = sizeof(clientaddr);
    sqlite3 *ppdb;
    int ret1 = sqlite3_open("./../../usr/usr.db",&ppdb); 
    if(ret1 != SQLITE_OK)
    {
        printf("sqlite3_open: %s\n",sqlite3_errmsg(ppdb));
    }

    //创建epoll对象
    int epollfd = epoll_create(MAXSIZE);
    if(-1 == epollfd)
    {
        perror("epoll_create");
        exit(1);
    }
    //printf("pollfd = %d\n",epollfd);

    struct epoll_event ev,ev1,events[MAXSIZE] = {0};
    ev.events = EPOLLIN;
    ev.data.fd = sockfd;
    int ret = epoll_ctl(epollfd,EPOLL_CTL_ADD,sockfd,&ev);  //注册事件：sockfd
    if(-1 == ret)
    {
        perror("epoll_ctl");
        exit(1);
    }
    //标准输入流监听
    ev1.events = EPOLLIN;
    ev1.data.fd = STDIN_FILENO;
    epoll_ctl(epollfd,EPOLL_CTL_ADD,STDIN_FILENO,&ev1);

    while (1)
    {
        //printf("epoll—wait等待事件到来！\n");
        int num = epoll_wait(epollfd,events,MAXSIZE,-1);  //等待事件的发生， -1:表示阻塞等待
        if(-1 == num)
        {
            perror("epoll_wait");
            exit(1);
        }
        for(int i = 0 ; i < num;i++)
        {
            if(events[i].data.fd == STDIN_FILENO) //服务器关机
            {

                char sql[N] = {0};//服务器关闭将所有用户在线fd置-1
                sprintf(sql,"update account set fd = '%d';",-1);
                sqlite_operate(ppdb,sql);

                fscanf(stdin,"%s",buf);
                //printf("ok\n");
                if(0 == strcmp("shutdown",buf))
                {
                    printf("服务器关闭!!!\n");
                    goto END;
                }
            }
            if(events[i].data.fd == sockfd)  //有客户端发起连接
            {
                //第五步：阻塞等待客户端的连接
                Client_INFO *client_info = (Client_INFO*)Accept_Client(sockfd);
                //printf("%p\n",client_info);
                acceptfd = client_info->fd;
                ev.events = EPOLLIN;
                ev.data.fd = acceptfd;
                // ret = epoll_ctl(epollfd,EPOLL_CTL_ADD,acceptfd,&ev);  //注册事件：sockfd
                // if(-1 == ret)
                // {
                //     perror("epoll_ctl");
                //     exit(1);
                // }
                
                //传参用结构体
                /*
                    typedef struct Client_INFO
                    {
                        int fd;
                        struct sockaddr_in clientaddr;
                    }Client_INFO;
                */
               /*   
                    typedef struct 
                    {
                        Client_INFO client;
                        sqlite3 *ppdb;
                    }ARG;
                */
                ARG arg;
                arg.client = *client_info;
                arg.ppdb = ppdb;
                pthread_t tid;
                if(pthread_create(&tid,NULL,func0,&arg) != 0)
                {
                    printf("线程创建失败！\n");
                    return -1;
                }
                if(0 != pthread_detach(tid))
                {
                    printf("设置非分离属性失败！\n");
                    return -1;
                }
                free(client_info);
            }
            #if 0
            else  //有客户端发消息
            {
                if(events[i].events & EPOLLIN)  //如果事件是可读的
                {
                    ARG arg;
                    arg.fd = events[i].data.fd;
                    arg.ppdb = ppdb;
                    pthread_t tid;
                    if(pthread_create(&tid,NULL,func0,&arg) != 0)
                    {
                        printf("线程创建失败！\n");
                        return -1;
                    }
                    if(0 != pthread_detach(tid))
                    {
                        printf("设置非分离属性失败！\n");
                        return -1;
                    }
                    //printf("%d\n",events[i].data.fd);
                    //func0(events[i].data.fd,ppdb);
                    //return 0;
                }
            }
            #endif
        }
        
    }
END:    
    sqlite3_close(ppdb);
    close(epollfd);
    close(sockfd);
    return 0;
}
