#include "sockcreate.h"
#include "myserver.h"
#include "sqlite_op.h"
//登录后的功能
void func1(sqlite3 *ppdb, const int acceptfd,const char *id)
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
            char sql[N] = {0};//客户端下线时将fd置为-1，表示离线状态
            sprintf(sql,"update account set fd = '%d' where id ='%s';",-1,id);
            sqlite_operate(ppdb,sql);
            //sqlite3_close(ppdb);

            //send(acceptfd,"END_CHAT",N,0);          
            //Quit_and_Save(ppdb,acceptfd,id);
            return ;
        }
        switch (choice)
        {
            //查看在线用户
            case VIEM_OL_MEMBER:
            {
                //void func(sqlite3 *ppdb,int acceptfd)
                char sql[N] = {0};
                sprintf(sql,"select * from account where fd > 0;");
                //接收查询结果
                RESULT *value = (RESULT *)sqlite_operate1(ppdb,sql);
                int Index = value->column;
                int n = 1;
                for(int i = 1;i <= value->row;i++)
                {
                        sprintf(buf,"\t%d.%s\n",n++,value->result[Index + 1]);
                        Index += value->column;
                        send(acceptfd,buf,N,0);
                        memset(buf,0,N);
                    //}
                }
                //释放查询函数开辟的malloc空间
                free(value);
                // send(acceptfd,"END",N,0);
                // memset(buf,0,sizeof(buf));
            }
            break;
            //私聊
            case PRIVATE_CHAT:
            {
                //void func(sqlite3 *ppdb,int acceptfd)
                Private_chat(ppdb,acceptfd,id);                
            }
            break;
            //群发
            case GROUP_CHAT:
            {
                //void func(sqlite3 *ppdb,int acceptfd,)
                Group_chat(ppdb,acceptfd,id);
            }
            break;
            //禁言(群聊)
            case SHUT_UP:
            {
                Ban_to_psot(ppdb,acceptfd,id);
            }
                break;
            //强制下线
            case KICK_OUT:
            {
                
            }
                break;
            case FILETR:
            {
                transfile(acceptfd);
            }
                break;
            default: 
            {
                char sql[N] = {0};//客户端下线时将fd置为-1，表示离线状态
                sprintf(sql,"update account set fd = '%d' where id ='%s';",-1,id);
                sqlite_operate(ppdb,sql);
                //sqlite3_close(ppdb);

                send(acceptfd,"END_CHAT",N,0);          
                Quit_and_Save(ppdb,acceptfd,id);
            }
                return;

        }
    }
    return;
}

