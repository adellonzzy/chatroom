#include "sqlite_op.h"
//检查账户是否已经存在
int CHECK_IF_EXIST(sqlite3 *ppdb,char *s,int mode)
{
    char sql[N] = {0};   
    char **result;
    int row,column;
    if(0 == mode)
    {
        sprintf(sql,"select *from account where id = '%s';",s);
        if(SQLITE_OK != sqlite3_get_table(ppdb,sql,&result,&row,&column,NULL))
        { 
            printf("sqlite3_get_table: %s\n",sqlite3_errmsg(ppdb));
        }
    }
    else if(1 == mode)
    {
        sprintf(sql,"select *from account where id = '%s';",s);
        if(SQLITE_OK != sqlite3_get_table(ppdb,sql,&result,&row,&column,NULL))
        {
            printf("sqlite3_get_table: %s\n",sqlite3_errmsg(ppdb));
        }
    }

    return row;
}
//登录账号密码验证
int LOGIN_CONFIRM(sqlite3 *ppdb,char *id,char *pw)
{
    char sql[N] = {0};   
    char **result;
    int row,column;
    sprintf(sql,"select *from account where id = '%s' and pw = '%s';",id,pw);
    if(SQLITE_OK != sqlite3_get_table(ppdb,sql,&result,&row,&column,NULL))
    {
        printf("sqlite3_get_table: %s\n",sqlite3_errmsg(ppdb));
    }
   // sqlite3_free_table(result);
    return row;
}
int LOGIN_CONFIRM_IF_LOGGEDIN(sqlite3 *ppdb,char *id,char *pw)
{
    char sql[N] = {0};   
    char **result;
    int row,column;
    sprintf(sql,"select *from account where id = '%s' and pw = '%s' and fd > 0;",id,pw);
    if(SQLITE_OK != sqlite3_get_table(ppdb,sql,&result,&row,&column,NULL))
    {
        printf("sqlite3_get_table: %s\n",sqlite3_errmsg(ppdb));
    }
   // sqlite3_free_table(result);
    return row;
}
//账号及密保认证
int ACCOUNT_CONFIRM(sqlite3 *ppdb,char *id,char *secret)
{
    char sql[N] = {0};   
    char **result;
    int row,column;
    sprintf(sql,"select *from account where id = '%s' and secret = '%s';",id,secret);
    if(SQLITE_OK != sqlite3_get_table(ppdb,sql,&result,&row,&column,NULL))
    {
        printf("sqlite3_get_table: %s\n",sqlite3_errmsg(ppdb));
    }
    return row;
}

//添加注册账户信息
void Insert_table(sqlite3 *ppdb,char *s1,char *s2,char *s3,int perm)
{
    char sql[N] = {0};
    sprintf(sql,"insert into account(fd,id,pw,secret,perm) values('%d','%s','%s','%s',%d);",-1,s1,s2,s3,perm);
    int ret = sqlite3_exec(ppdb,sql,NULL,NULL,NULL);
    if(ret != SQLITE_OK)
    {
        printf("sqlite3_exec: %s\n",sqlite3_errmsg(ppdb));
    }
    return ;
}
//修改密码
void Edit_pw(sqlite3 *ppdb,char *id,char *pw)
{
    char sql[N] = {0};
    sprintf(sql,"update account set pw = '%s' where id = '%s';",pw,id);
    int ret = sqlite3_exec(ppdb,sql,NULL,NULL,NULL);
    if(ret != SQLITE_OK)
    {
        printf("sqlite3_exec: %s\n",sqlite3_errmsg(ppdb));
    }
    return ;
}




#if 1
//自定义操作1
void sqlite_operate(sqlite3 *ppdb,char *sql)
{
    int ret = sqlite3_exec(ppdb,sql,NULL,NULL,NULL);
    if(ret != SQLITE_OK)
    {
        printf("sqlite3_exec: %s\n",sqlite3_errmsg(ppdb));
    }
    return ;

} 
#endif
//自定义操作2
void *sqlite_operate1(sqlite3 *ppdb,char *sql)
{
    RESULT *value = (RESULT *)malloc(sizeof(RESULT));
    int ret = sqlite3_get_table(ppdb,sql,&value->result,&value->row,&value->column,NULL);
    if(ret != SQLITE_OK)
    {
        printf("sqlite3_exec: %s\n",sqlite3_errmsg(ppdb));
    }
    //sqlite3_free_table(value->result);
    return value;

} 

#if 0
int main(int argc, char const *argv[])
 {
    sqlite3 *ppdb;
    int ret = sqlite3_open("./usr/usr.db",&ppdb); 
    if(ret != SQLITE_OK)
    {
        printf("sqlite3_open: %s\n",sqlite3_errmsg(ppdb));
    }

    printf("%d\n",CHECK_IF_EXIST(ppdb,"shan"));

    return 0;
 }
 #endif
 