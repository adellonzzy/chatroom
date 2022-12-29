#include "sockcreate.h"
#include "myserver.h"
#include "sqlite_op.h"
void Private_chat(sqlite3 *ppdb,const int acceptfd,const char *id)
{
    char buf[N] = {0};
    recv(acceptfd,buf,N,0);
    char sql[N] = {0};
    sprintf(sql,"select * from account where fd > 0 and id = '%s';",buf);
    RESULT *value = (RESULT *)sqlite_operate1(ppdb,sql);
    //printf("value->column = %d\n",value->column);
    if(0 == value->row)
    {
        send(acceptfd,"OFFLINE",N,0);
    }
    else
    {
        send(acceptfd,"SUCCESS",N,0);
        int recvfd = atoi(value->result[value->column]);

        char name[ID_MAX] = {0};
        strcpy(name,buf);//接收方id保存

        //printf("recvfd = %d\n",recvfd);
        free(value);//获取接收人fd后释放malloc空间
        
        if(recvfd != acceptfd)
        {
            //send(acceptfd,"success",N,0);
            char buf1[N] = {0};
            while (1)
            {
                memset(buf,0,N);
                //开始聊天
                int num = recv(acceptfd,buf,N,0);
                if(num == 0)
                {
                    //send(recvfd,"QUIT",N,0);
                    break;
                }   
                if(0 == strcmp("quit",buf))
                {
                    //send(recvfd,"QUIT",N,0);
                    break;
                }                        
                //存聊天记录
                memset(sql,0,N);
                sprintf(sql,"insert into record (send,recv,content) values('%s','%s','%s');",id,name,buf);
                sqlite_operate(ppdb,sql);

                sprintf(buf1,"%s : %s",id,buf); 
                send(recvfd,buf1,N,0);
                memset(buf,0,N);
            }         
        }
        else
        {
            send(acceptfd,"FAILURE",N,0);
        }
    }
    return;
}