#include "sockcreate.h"
#include "myclient.h"

//#define IP_addr "192.168.59.176"
#define IP_addr "127.0.0.1"
#define Port_num "8888"

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
        int choice = 0;

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
            if(-1 == send(sockfd,&choice,sizeof(int),0))
            {
                perror("send");
                return -1;
            }
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
                    //转至登录后的界面
                    Func_choose(sockfd,account.id);                    
                    break;
                }
                printf("%s\n",buf);
                memset(buf,0,sizeof(buf)); 
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
            //goto END;
        }

    }
// END:
    close(sockfd);
    return 0;
}