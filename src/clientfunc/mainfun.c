#include "sockcreate.h"
#include "myclient.h"
void *chatrecv(void *arg)
{
    int sockfd = *(int *)arg;//保存服务器通信的文件描述符
    int *p = (int *)arg;//指针p指向arg用于后续线程间的通讯
    //printf("chat recv :%p\n",p);
    char buf[N] = {0};
    while (1)
    {
        recv(sockfd,buf,N,0);
        if(*p == FILETR)
        {
            pthread_cond_signal(&cond);
            sleep(1);
            pthread_cond_wait(&cond,&mutex);
            *p = 0;
            continue;
        }
        if(0 == strcmp("SHUT_UP",buf))//被禁言
        {
            *p = SHUT_UP;
            sprintf(buf,"你已被管理员禁言！");
        }
        else if(0 == strcmp("OFFLINE",buf))//私聊对象不在线或不存在
        {
            *p = OFFLINE;
            pthread_cond_signal(&cond);
            continue;
        }
        else if(0 == strcmp("FAILURE",buf))//私聊对象为自己
        {
            *p = FAILURE;
            pthread_cond_signal(&cond);
            continue;
        }

        if(0 == strcmp("KICK_OUT",buf))//被踢下线
        {
            *p = KICK_OUT;
            exit(0);
            //pthread_exit(NULL);
        }
        if(*p == END_CHAT)//退出聊天功能
        {
            pthread_cond_signal(&cond);
            pthread_exit(NULL);
        }
        pthread_cond_signal(&cond);
        printf("%s\n",buf);
        memset(buf,0,N);
    }             

}
void Func_choose(int sockfd,const char *id)
{
    pthread_t tid;//创建用于接收消息的线程
    int arg = sockfd;
    if(pthread_create(&tid,NULL,chatrecv,&arg) != 0)
    {
        printf("线程创建失败！\n");
        return ;
    }
    if(0 != pthread_detach(tid))
    {
        printf("设置非分离属性失败！\n");
        return ;
    }

    while (1)
    {
        //system("clear");
        Menu2();

        int choice;
        char buf[N] = {0};
        // while(getchar()!='\n')
        //     continue;        
        scanf("%d",&choice);

        switch (choice)
        {   
            //查看在线用户
            case 1:
            {
                choice = VIEM_OL_MEMBER;
                system("clear");
                send(sockfd,&choice,sizeof(int),0);
                //接收服务器发来的在线用户信息
                
                // while(0 <  recv(sockfd,buf,N,0))
                {
                    // if(0 == strcmp("END",buf))
                    // {
                char end;
                scanf(" %c",&end);
              // break;
                    // }
                    // printf("%s\n",buf);
                    // memset(buf,0,N);
                }
            }
            break;
            case 2:
            {
                choice = PRIVATE_CHAT;
                send(sockfd,&choice,sizeof(int),0);
                Private_chat(sockfd,&arg);
            }
                break;
            //群发
            case 3:
            {
                choice = GROUP_CHAT;
                send(sockfd,&choice,sizeof(int),0);
                Group_chat(sockfd,&arg);          
            }
                break;
            case 4:
            {
                choice = SHUT_UP;
                send(sockfd,&choice,sizeof(int),0);
                Ban_to_psot(sockfd);
                sleep(1);
            }
                break;
            case 5:
                //choice = GROUP_CHAT;
                send(sockfd,&choice,sizeof(int),0);

                break;
            case 6:
            {
                choice = FILETR;
                send(sockfd,&choice,sizeof(int),0);
                transfile(sockfd,&arg);
            }
                break;
            case 7:
                send(sockfd,&choice,sizeof(int),0);
                Quit_and_Save(sockfd,id,&arg);
                return;
            default:
                while(getchar()!='\n')
                    continue;   
                break;
        }

    }

    return;
}