#ifndef _SQLITEOP_H_
#define _SQLITEOP_H_
#include <sqlite3.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define N 128

//查询结果结构体
typedef struct RESULT
{
    char **result;
    int row;
    int column;
}RESULT;

//检查是否已经存在
int CHECK_IF_EXIST(sqlite3 *ppdb,char *s,int mode);
//添加账号信息
void Insert_table(sqlite3 *ppdb,char *s1,char *s2,char *s3,int perm);
//登录账号密码验证
int LOGIN_CONFIRM(sqlite3 *ppdb,char *id,char *pw);
//是否已经登录，防止重复登录验证
int LOGIN_CONFIRM_IF_LOGGEDIN(sqlite3 *ppdb,char *id,char *pw);
//账号及密保认证
int ACCOUNT_CONFIRM(sqlite3 *ppdb,char *id,char *pw);
//修改密码
void Edit_pw(sqlite3 *ppdb,char *id,char *pw);
//自定义操作1
void sqlite_operate(sqlite3 *ppdb,char *sql);
//自定义操作2
void *sqlite_operate1(sqlite3 *ppdb,char *sql);

/*
CREATE TABLE record(send char,recv char,time char,content txt);
CREATE TABLE account(fd int,id char,nickname char,pw char,secret char,perm int);
*/
#endif