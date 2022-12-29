#include "sockcreate.h"
#include "myserver.h"
#include "sqlite_op.h"
int Print(void *arg,int ncolumn,char**f_value,char**f_name) 
{
    //printf("success\n");
    char buf[N] = {0};
    int acceptfd = *(int *)arg;
    //CREATE TABLE record(send char,recv char,time char,content txt);
    //for(int i = 0 ; i < ncolumn;i++)
    {
        sprintf(buf,"%s to %s :\n\t2\t%s\n",f_value[0],f_value[1],f_value[3]);
        send(acceptfd,buf,N,0);
        memset(buf,0,N);
    }
    return 0;
}
void Quit_and_Save(sqlite3 *ppdb,const int acceptfd,const char *id)
{
    char sql[N] = {0};
    int arg = acceptfd;
    sprintf(sql,"select * from record where send = '%s' or recv = '%s';",id,id);
    int ret = sqlite3_exec(ppdb,sql,Print,&arg,NULL);
    if(ret != SQLITE_OK)
    {
        printf("sqlite3_exec: %s\n",sqlite3_errmsg(ppdb));
    }
    send(acceptfd,"finished",N,0);
    return;
}