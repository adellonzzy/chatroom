#include "sockcreate.h"
#include "myserver.h"
#include "sqlite_op.h"
//登录后的功能
void func1(sqlite3 *ppdb, const int acceptfd,const char *id)
//void *func1(void *arg)
{
    while(1)
    {
        int choice;
        char buf[N] = {0};

        int bytes;

        bytes = recv(acceptfd,&choice,sizeof(int),0);
        if(bytes == -1)
        {
            perror("recv");
            return;
        }
        else if(bytes == 0)//客户端退出
        {
            return ;
        }
        //memset(buf,0,sizeof(buf));
        //send(acceptfd,"ACCESS_PERMITTED",N,0);
        switch (choice)
        {
            //查看在线用户
            case VIEM_OL_MEMBER:
            {
                //void func(sqlite3 *ppdb,int acceptfd)

                //recv(acceptfd,buf,N,0);
                char sql[N] = {0};
                sprintf(sql,"select * from account where fd > 0;");
                //接收查询结果
                RESULT *value = (RESULT *)sqlite_operate1(ppdb,sql);
                //*value = *(RESULT *)sqlite_operate1(ppdb,sql);
                int Index = value->column;
                //CREATE TABLE account(fd int,id char,pw char,secret char,perm int);
                int n = 1;
                for(int i = 1;i <= value->row;i++)
                {
                    //for(int j = 0;j < value->column;j++)
                    //{
                        //sprintf(buf,"success\n");
                        sprintf(buf,"\t%d.%s\n",n++,value->result[Index + 1]);
                        Index += value->column;
                        send(acceptfd,buf,N,0);
                        memset(buf,0,N);
                    //}
                }
                //释放查询函数开辟的malloc空间
                free(value);
                send(acceptfd,"END",N,0);
                // memset(buf,0,sizeof(buf));
            }
            break;
            //私聊
            case PRIVATE_CHAT:
            {
                //void func(sqlite3 *ppdb,int acceptfd)

                recv(acceptfd,buf,N,0);
                char sql[N] = {0};
                sprintf(sql,"select * from account where fd > 0 and id = '%s';",buf);
                RESULT *value = (RESULT *)sqlite_operate1(ppdb,sql);
                //printf("value->column = %d\n",value->column);

                if(0 == value->row)
                {
                    send(acceptfd,"对方已离线。",N,0);
                }
                else
                {
                    int recvfd = atoi(value->result[value->column]);
                    //printf("recvfd = %d\n",recvfd);
                    free(value);//获取接收人fd后释放malloc空间
                    
                    if(recvfd != acceptfd)
                    {
                        send(acceptfd,"success",N,0);
                        // printf("fd =  %d\n",recvfd);
                        // printf("acceptfd = %d\n",acceptfd);
                        while (1)
                        {
                            int num = recv(acceptfd,buf,N,0);
                            if(num == 0)
                            {
                                break;
                            }                           
                            //memset(buf,0,N);
                            send(recvfd,buf,N,0);
                            memset(buf,0,N);
                        }
                    }
                    else
                    {
                        send(acceptfd,"不能给自己发消息！",N,0);
                    }
                }
                
            }
            break;
            //群发
            case GROUP_CHAT:
            {
                char sql[N] = {0};
                sprintf(sql,"select * from account where fd > 0;");
                //接收查询结果
                RESULT *value = (RESULT *)sqlite_operate1(ppdb,sql);
                int recvfd;
                while (1)
                {
                    int Index = value->column;
                    int num = recv(acceptfd,buf,N,0);
                    if(0 == num)
                    {
                        printf("%s退出群聊。\n",id);
                        break;
                    }
                    for(int i = 1;i <= value->row;i++)
                    {
                        recvfd = atoi(value->result[Index]);
                        if(acceptfd != recvfd)
                        {
                            send(recvfd,buf,N,0);
                        }
                        Index += value->column;
                    }
                    memset(buf,0,N);
                }   
            }
            break;
            default:return;
        }
    }
    return;
}

//注册登录功能
//void func0(int acceptfd,sqlite3 *ppdb)
void *func0(void *arg)
{
    ARG arg0 = *(ARG*)arg;
    int acceptfd = arg0.client.fd;
    //printf("%d\n",i->fd);
    sqlite3 *ppdb = arg0.ppdb;
    int choice;
    char buf[N] = {0};
    ssize_t bytes;   
    //账号信息结构体
    account_info account;
    memset(&account,0,sizeof(account));

    while(1)
    {
        bytes = recv(acceptfd,&choice,sizeof(int),0);
        if(bytes == -1)
        {
            ERRLOG("recv");
        }
        else if(bytes == 0)//客户端退出
        {
            //printf("客户端已下线\n");
            printf("客户端%s:%d已下线\n",inet_ntoa(arg0.client.clientaddr.sin_addr),ntohs(arg0.client.clientaddr.sin_port));

            char sql[N] = {0};//客户端下线时将fd置为-1，表示离线状态
            sprintf(sql,"update account set fd = '%d' where id ='%s';",-1,account.id);
            sqlite_operate(ppdb,sql);
            //sqlite3_close(ppdb);
            pthread_exit(NULL);
            return NULL;
        }
        memset(buf,0,sizeof(buf));
        switch (choice)
        {
        //账号注册
        case SIGN_UP:
        {
            //预留位
            send(acceptfd,"ACCESS_PERMITTED",N,0);
            while (1)
            {
                //memset(buf,0,sizeof(buf));

                recv(acceptfd,&account,sizeof(account),0);
                //id重复检测
                if(0 < CHECK_IF_EXIST(ppdb,account.id,0))
                {
                    send(acceptfd,"该账号已被注册!",N,0);
                    break;
                }
                else
                {
                    Insert_table(ppdb,account.id,account.pw,account.secret,1);
                    send(acceptfd,"success",N,0);
                    break;
                }            
            }
        }
            break;
        //账号登录
        case SIGN_IN:
        {
            //预留位
            // if()
            // {

            // }
            send(acceptfd,"ACCESS_PERMITTED",N,0);
            while (1)
            {
                recv(acceptfd,&account,sizeof(account),0);
                if(1 ==  LOGIN_CONFIRM_IF_LOGGEDIN(ppdb,account.id,account.pw))
                {
                    send(acceptfd,"账号已经登录！",N,0);
                }
                else if(1 ==  LOGIN_CONFIRM(ppdb,account.id,account.pw))
                {
                    send(acceptfd,"success",N,0);
                    //数据库分配fd信息
                    if(0 < CHECK_IF_EXIST(ppdb,account.id,1))//如果已在fd表中那么更新该账号fd的值
                    {
                        char sql[N] = {0};
                        sprintf(sql,"update account set fd = '%d' where id ='%s';",acceptfd,account.id);
                        sqlite_operate(ppdb,sql);
                    }
                    else//如果不在表中则添加该用户的fd信息
                    {
                        char sql[N] = {0};
                        sprintf(sql,"insert into account(fd,id,perm) values(%d,'%s',%d);",acceptfd,account.id,1);
                        sqlite_operate(ppdb,sql);
                    }
                    //转到登录后的功能
                    func1(ppdb,acceptfd,account.id);
                    break;
                }
                else
                {
                    send(acceptfd,"账号或密码错误！",N,0);
                    break;
                }
            }

        }
            break;
        //忘记密码
        case FORGET_PW:
        {
            //预留位
            send(acceptfd,"ACCESS_PERMITTED",N,0);
            while (1)
            {
                recv(acceptfd,&account,sizeof(account),0);
                //账号密保认证
                if(1 ==  LOGIN_CONFIRM(ppdb,account.id,account.secret))
                {
                    send(acceptfd,"success",N,0);
                    recv(acceptfd,&account,sizeof(account),0);
                    Edit_pw(ppdb,account.id,account.pw);
                    send(acceptfd,"密码修改成功！",N,0);
                    break;
                }
                else
                {
                    send(acceptfd,"账号不存在或密保有误！",N,0);
                    break;
                }
            }
        }
            break;
        default:
            break;
        }

    }
    pthread_exit(NULL);
    return NULL;
}