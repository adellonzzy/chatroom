#include "sockcreate.h"
#include "myserver.h"
#include "sqlite_op.h"
int Group_send(void *arg,int ncolumn,char**f_value,char**f_name)
{
    char *buf = (char *)arg;
    //CREATE TABLE account(fd int,id char,pw char,secret char,perm int);
    send(atoi(f_value[0]),buf,N,0);
    
    return 0;
}

#if 1
void Group_chat(sqlite3 *ppdb,const int acceptfd,const char *id)
{
    char buf[N] = {0};
    char buf1[N] = {0};
    char sql[N] = {0};
    int ret;
    sprintf(sql,"select * from account where fd > 0 and fd != '%d';",acceptfd);
    while (1)
    {
        int num = recv(acceptfd,buf,N,0);//接收发送者的消息
        if(0 == num || 0 == strcmp("quit",buf)) //客户端退出或结束群聊
        {
            if(0 == num)
            {
                sprintf(buf,"%s下线了。",id);
            }
            else if(0 == strcmp("quit",buf))
            {
                sprintf(buf,"%s退出了群聊。",id);
            }
            
            ret = sqlite3_exec(ppdb,sql,Group_send,buf,NULL);
            if(ret != SQLITE_OK)
            {
                printf("sqlite3_exec: %s\n",sqlite3_errmsg(ppdb));
            }
            memset(buf,0,N);

            break;
        }
        //群发消息给其他用户
        sprintf(buf1,"%s : %s",id,buf);
        ret = sqlite3_exec(ppdb,sql,Group_send,buf1,NULL);
        if(ret != SQLITE_OK)
        {
            printf("sqlite3_exec: %s\n",sqlite3_errmsg(ppdb));
        }
        memset(buf,0,N);
        memset(buf1,0,N);
    }
    return ;
}
#endif