#include "sockcreate.h"
#include "myserver.h"
#include "sqlite_op.h"
void Ban_to_psot(sqlite3 *ppdb,int acceptfd,const char *id)
{
    char buf[N] = {0};
    recv(acceptfd,buf,N,0);
    char sql[N] = {0};   
    char **result;
    int row,column;
    sprintf(sql,"select *from account where id = '%s' and perm = 999;",id);

    if(SQLITE_OK != sqlite3_get_table(ppdb,sql,&result,&row,&column,NULL))
    {
        printf("sqlite3_get_table: %s\n",sqlite3_errmsg(ppdb));
    }
    memset(sql,0,N);
    if(row > 0)
    {
        sprintf(sql,"select *from account where id = '%s';",buf);
        if(SQLITE_OK != sqlite3_get_table(ppdb,sql,&result,&row,&column,NULL))
        {
            printf("sqlite3_get_table: %s\n",sqlite3_errmsg(ppdb));
        }
        if(row > 0)
        {
            send(atoi(result[column]),"SHUT_UP",N,0);
            send(acceptfd,"已将该用户禁言！",N,0);
        }
        else
        {
            send(acceptfd,"此用户不存在！",N,0);
        }
    }
    else
    {
        send(acceptfd,"你没有该权限！",N,0);
    }
    sqlite3_free_table(result);
    return;
}
