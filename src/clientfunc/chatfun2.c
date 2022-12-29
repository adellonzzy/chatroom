#include "sockcreate.h"
#include "myclient.h"
void Group_chat(int sockfd,void *arg)
{
    char buf[N] = {0};
    system("clear");
    int *p = (int *)arg;
    // pthread_t tid;//创建用于接收消息的线程
    // if(pthread_create(&tid,NULL,chatrecv,&sockfd) != 0)
    // {
    //     printf("线程创建失败！\n");
    //     return ;
    // }
    // if(0 != pthread_detach(tid))
    // {
    //     printf("设置非分离属性失败！\n");
    //     return ;
    // }
    printf("进入群聊请输入发送内容。\n");

    while (1)
    {
        scanf("%s",buf);
        if(*p == SHUT_UP)
        {          
            while(1)
            {
                printf("您已被禁言！\n");
                scanf("%s",buf);
                if(0 == strcmp("quit",buf))
                {
                    send(sockfd,buf,N,0);
                    return;
                }   
                memset(buf,0,N);               
            }
        }

        send(sockfd,buf,N,0);
        if(0 == strcmp("quit",buf))
        {
            
            //pthread_cancel(tid);
            break;
        }
        //send(sockfd,buf,N,0);
        memset(buf,0,N);
    }
    return ; 
}
