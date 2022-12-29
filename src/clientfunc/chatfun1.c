#include "sockcreate.h"
#include "myclient.h"

void Private_chat(int sockfd,void *arg)
{
    char buf[N] = {0};
    printf("输入聊天对象的ID\n");
    scanf("%s",buf);
    send(sockfd,buf,N,0);
    memset(buf,0,N);

    int *p = (int *)arg;
    //等待chat_recv先执行发出指令后再继续
    pthread_cond_wait(&cond,&mutex);

    if (OFFLINE == *p)
    {
        printf("该用户已离线。\n");
        sleep(2);
        return;
    }
    if (FAILURE == *p)
    {
        printf("不能和自己聊天！\n");
        sleep(2);
        return;
    }
    //printf("success\n");
    *p = 0;//指令复位
    //else
    {
        memset(buf,0,N);
        system("clear");

        while (1)
        {
            scanf("%s",buf);
            send(sockfd,buf,N,0);
            if(0 == strcmp("quit",buf))
            {
                //pthread_cancel(tid);
                break;
            }
            memset(buf,0,N);
        }             
    }
    return ;
}