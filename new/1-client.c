#include "sockcreate.h"
#include "myserver.h"
#include "sqlite_op.h"
#define IP_addr "192.168.59.166"
#define Port_num "8888"
void Menu1()
{
	system("clear");
	printf("------------------------------------------\n\n");
	printf("\t\t1.注册\n\n");
	printf("\t\t2.登录\n\n");
	printf("\t\t3.忘记密码\n\n");
	printf("------------------------------------------\n\n");
}
void Menu2()
{
	system("clear");
	printf("------------------------------------------\n\n");
	printf("\t\t1.查看在线用户\n\n");
	printf("\t\t2.私聊\n\n");
	printf("\t\t3.群聊\n\n");
    printf("\t\t4.禁言(admin权限)\n\n");
	printf("\t\t5.强制下线(admin权限)\n\n");
	printf("\t\t6.登出\n\n");

	printf("------------------------------------------\n\n");
}

void *chatrecv(void *arg)
{
    int sockfd = *(int *)arg;
    char buf[N] = {0};
    while (1)
    {
        recv(sockfd,buf,N,0);
        printf("%s\n",buf);
        memset(buf,0,N);
    }             

}

void Chat_func(int sockfd)
{
    while (1)
    {
        system("clear");
        Menu2();
        int choice;
        char buf[N] = {0};
        // while(getchar()!='\n')
        //     continue;        
        scanf("%d",&choice);
        //send(sockfd,&choice,sizeof(int),0);
        //recv(sockfd,buf,N,0);
        // if(0 != strcmp("OK",buf))
        // {
        //     return;
        // }
        switch (choice)
        {   
            //查看在线用户
            case 1:
            {
                //void func(int sockfd)
                choice = VIEM_OL_MEMBER;
                send(sockfd,&choice,sizeof(int),0);
                while(0 <  recv(sockfd,buf,N,0))
                {
                    if(0 == strcmp("END",buf))
                    {
                        char end;
                        scanf(" %c",&end);
                        break;
                    }
                    printf("%s\n",buf);
                    memset(buf,0,N);
                }
            }
            break;
            //私聊
            case 2:
            {
                //void func(int sockfd)
                choice = PRIVATE_CHAT;
                send(sockfd,&choice,sizeof(int),0);

                printf("输入聊天对象的ID\n");
                scanf("%s",buf);
                send(sockfd,buf,N,0);
                memset(buf,0,N);

                recv(sockfd,buf,N,0);
                if (0 != strcmp("success",buf))
                {
                    printf("%s\n",buf);
                    sleep(2);
                    break;
                }
                //printf("%s\n",buf);
                memset(buf,0,N);
                sleep(2);
                system("clear");

                pthread_t tid;
                if(pthread_create(&tid,NULL,chatrecv,&sockfd) != 0)
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
                    scanf("%s",buf);
                    send(sockfd,buf,N,0);
                    if(0 == strcmp("quit",buf))
                    {
                        pthread_cancel(tid);
                        break;
                    }
                    memset(buf,0,N);
                }             
            }
            break;
            //群发
            case 3:
            {
                choice = GROUP_CHAT;
                send(sockfd,&choice,sizeof(int),0);
                
                system("clear");

                pthread_t tid;
                if(pthread_create(&tid,NULL,chatrecv,&sockfd) != 0)
                {
                    printf("线程创建失败！\n");
                    return ;
                }
                if(0 != pthread_detach(tid))
                {
                    printf("设置非分离属性失败！\n");
                    return ;
                }
                printf("您已进入群聊\n");
                while (1)
                {
                    scanf("%s",buf);
                    send(sockfd,buf,N,0);
                    if(0 == strcmp("quit",buf))
                    {
                        pthread_cancel(tid);
                        break;
                    }
                    memset(buf,0,N);
                }             
            }
                break;
            case 6:
                return;
            default:
                while(getchar()!='\n')
                    continue;        
                break;
        }

    }
    return;
}
int main(int argc, char const *argv[])
{
    int sockfd = Sockfd_Create();
    char IP[] = IP_addr;
    char Port[] = Port_num;
    Connect_Server(sockfd,IP,Port);
    int choice;
    char buf[N] = {0};
    while (1)
    {
        Menu1();
        // while(getchar()!='\n')
        //     continue;        

        scanf("%d",&choice);
        switch (choice)
        {
        case 1://账号注册
        {
            choice = SIGN_UP;
            send(sockfd,&choice,sizeof(int),0);
            recv(sockfd,buf,N,0);
            if(0 != strcmp("ACCESS_PERMITTED",buf))
            {
                printf("暂未开放注册！\n");
                sleep(2);
                break;
            }
            memset(buf,0,sizeof(buf)); 
            account_info account;  
            while(1)
            {
                memset(&account,0,sizeof(account)); 
                printf("请输入账户：\n");
                scanf("%s",account.id);
                printf("请输入密码：\n");
                scanf("%s",account.pw);
                printf("请输入密保：\n");
                scanf("%s",account.secret);
                send(sockfd,&account,sizeof(account),0);
                recv(sockfd,buf,N,0);
                //服务器返回查询结果
                if(0 == strcmp("success",buf))
                {
                    printf("注册成功！\n");
                    memset(buf,0,sizeof(buf));                         
                    sleep(2);
                    break;
                }
                printf("%s\n",buf);
                memset(buf,0,sizeof(buf)); 
                sleep(2);
                break;
            }
        }
            break;
        case 2://账号登录
        {
            choice = SIGN_IN;
            send(sockfd,&choice,sizeof(int),0);
            recv(sockfd,buf,N,0);
            if(0 != strcmp("ACCESS_PERMITTED",buf))
            {
                printf("您的账号已被限制！\n");
                sleep(2);
                break;
            }
            memset(buf,0,sizeof(buf)); 
            account_info account;  
            while(1)
            {
                memset(&account,0,sizeof(account)); 
                printf("请输入账户：\n");
                scanf("%s",account.id);
                printf("请输入密码：\n");
                scanf("%s",account.pw);
                send(sockfd,&account,sizeof(account),0);
                recv(sockfd,buf,N,0);
                //服务器返回查询结果
                if(0 == strcmp("success",buf))
                {
                /*
                    printf("登录成功！\n");
                    memset(buf,0,sizeof(buf));                         
                    sleep(2);
                */    
                    Chat_func(sockfd);
                    break;
                }
                printf("%s\n",buf);
                memset(        //Menu2();
buf,0,sizeof(buf)); 
                sleep(2);
                break;
            }
        }
            break;
        case 3://忘记密码
        {
            choice = FORGET_PW;
            send(sockfd,&choice,sizeof(int),0);
            recv(sockfd,buf,N,0);
            if(0 != strcmp("ACCESS_PERMITTED",buf))
            {
                //printf("您的账号已被限制\n");
                sleep(2);
                break;
            }
            memset(buf,0,sizeof(buf)); 
            account_info account;  
            while(1)
            {
                memset(&account,0,sizeof(account)); 
                printf("请输入账户：\n");
                scanf("%s",account.id);
                printf("请输入密保：\n");
                scanf("%s",account.secret);
                send(sockfd,&account,sizeof(account),0);
                recv(sockfd,buf,N,0);
                //服务器返回查询结果
                if(0 != strcmp("success",buf))
                {
                    printf("%s\n",buf);
                    memset(buf,0,sizeof(buf)); 
                    sleep(2);
                    break;
                }
                //while (1)
                {
                    memset(buf,0,sizeof(buf)); 
                    printf("请输入您的新密码！\n");
                    scanf("%s",account.pw);
                    send(sockfd,&account,sizeof(account),0);
                    recv(sockfd,buf,N,0);
                    printf("%s\n",buf);
                    memset(buf,0,sizeof(buf)); 
                    sleep(2);
                    break;
                }
            }
        }
            break;
        default:
            return 0;
        }

    }
    
    return 0;
}