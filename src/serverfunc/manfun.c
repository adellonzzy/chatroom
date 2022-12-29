#include "sockcreate.h"
#include "myserver.h"
#include "sqlite_op.h"
//注册登录功能
//void func0(int acceptfd,sqlite3 *ppdb)
void *func0(void *arg)
{
    //存储传输来的数据库操作指针和客户端通信文件描述符
    ARG arg0 = *(ARG*)arg;
    int acceptfd = arg0.client.fd;
    sqlite3 *ppdb = arg0.ppdb;
    //int choice;
    char buf[N] = {0};
    ssize_t bytes;   
    //账号信息结构体
    account_info account;
    memset(&account,0,sizeof(account));

    while(1)
    {
        int choice = 0;
        bytes = recv(acceptfd,&choice,sizeof(int),0);

        if(bytes == -1)
        {
            ERRLOG("recv");
        }
        else if(bytes == 0)//客户端退出
        {
            //printf("客户端已下线\n");
            printf("客户端%s:%d已下线\n",inet_ntoa(arg0.client.clientaddr.sin_addr),ntohs(arg0.client.clientaddr.sin_port));

            // char sql[N] = {0};//客户端下线时将fd置为-1，表示离线状态
            // sprintf(sql,"update account set fd = '%d' where id ='%s';",-1,account.id);
            // sqlite_operate(ppdb,sql);
            // sqlite3_close(ppdb);
            // pthread_exit(NULL);
            // return NULL;
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
                    //goto END;
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
            goto END;
            break;
        }
    }
END:
    pthread_exit(NULL);
    //return NULL;
}