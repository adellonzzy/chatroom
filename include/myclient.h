#ifndef _MY_CLIENT_H_
#define _MY_CLIENT_H_
#define ID_MAX 16
#define PW_MAX 24
#define N 128
#include <stdio.h>
#include <stdlib.h>

pthread_mutex_t mutex;
pthread_cond_t cond;
//extern void *arg;

enum CONFIRM
{
    SIGN_UP = 10,//注册
    SIGN_IN,//登录
    FORGET_PW,//忘记密码
    
    VIEM_OL_MEMBER = 20,//查看在线用户
    PRIVATE_CHAT,//私聊
    GROUP_CHAT,//群聊
    SHUT_UP,//禁言
    KICK_OUT,//强制下线
    FILETR,



    //线程通信用指令
    SUCCESS = 50,
    FAILURE,
    QUIT,
    OFFLINE,
    ONLINE,
    END_CHAT,

    NONE
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


//************客户端**************//
void Menu1();//界面1
void Menu2();//界面2
//功能选择
void Func_choose(int sockfd,const char *id);
//客户端消息接收用线程
void *chatrecv(void *arg);
//私聊
void Private_chat(int sockfd,void *arg);
//群聊
void Group_chat(int sockfd,void *arg);
//
void Quit_and_Save(int sockfd,const char *id,void *arg);
//
void Ban_to_psot(int sockfd);
//
void transfile(int sockfd,void *arg);

#endif
