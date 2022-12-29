#ifndef _MY_SERVER_H_
#define _MY_SERVER_H_

#include <sqlite3.h>

#define ID_MAX 16
#define PW_MAX 24
#define TIME_MAX 32
#define MSG_MAX 256

pthread_mutex_t mutex;
pthread_cond_t cond;

enum CONFIRM
{
    BANNED = -999,
    
    BANNED_TO_POST = -1,
    NOT_EXISTS = 0,
    NORMAL_PERM = 1,

    SIGN_UP = 10,//注册
    SIGN_IN,//登录
    FORGET_PW,//忘记密码
    
    VIEM_OL_MEMBER = 20,//查看在线用户
    PRIVATE_CHAT,//私聊
    GROUP_CHAT,//群聊
    SHUT_UP,//禁言
    KICK_OUT,//强制下线
    FILETR,

    //客户端线程通信指令
    SUCCESS = 50,
    FAILURE,
    QUIT,
    OFFLINE,
    ONLINE,
    END_CHAT,


    ACCESS_DENIED = 100,
    ACCESS_PERMITTED,

    VIP = 999


};
//账户信息结构体
typedef struct account_info
{
    char id[ID_MAX];
    char pw[PW_MAX];
    char secret[PW_MAX];
    //char nickname[ID_MAX];
    //int fd;
}account_info;
//聊天消息结构体
typedef struct MSG
{
    char sender[ID_MAX];
    char receiver[ID_MAX];
    char time[TIME_MAX];
    char content[MSG_MAX];
}MSG;
//服务器给线程传参用结构体
typedef struct 
{
    //int fd;
    Client_INFO client;
    sqlite3 *ppdb;
    //char id[ID_MAX];
}ARG;
//群聊时各用户文件描述符存储
typedef struct GROUP
{
    int fd;
    char id[ID_MAX];
    struct GROUP *next;
}GROUP;


//void func0(int acceptfd,sqlite3 *ppdb);
//服务器与单客户端交互线程
void *func0(void *arg);
//登录后的功能
void func1(sqlite3 *ppdb, const int acceptfd,const char *id);
//私聊
void Private_chat(sqlite3 *ppdb,const int acceptfd,const char *id);
//群聊
void Group_chat(sqlite3 *ppdb,const int acceptfd,const char *id);
//客户端退出将聊天记录发送并保存
void Quit_and_Save(sqlite3 *ppdb,const int acceptfd,const char *id);
//群发给在线用户
int Group_send(void *arg,int ncolumn,char**f_value,char**f_name);
//
void Ban_to_psot(sqlite3 *ppdb,int acceptfd,const char *id);
//
void transfile(int acceptfd);

#endif
