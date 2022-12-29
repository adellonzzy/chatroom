#include "sockcreate.h"
#include "myclient.h"
void transfile(int sockfd,void *arg)
{
    //pthread_mutex_lock(&mutex);
    int *p = (int *)arg;
    printf("输入要接收的文件的名字\n");
    char *buf = (char *)malloc(sizeof(char)*1024);
    //char buf[1024] = {0};
    scanf("%s",buf);
    send(sockfd,buf,N,0);
    *p = FILETR;
    pthread_cond_wait(&cond,&mutex);

    FILE *fd1 = fopen(buf,"w");
    int num;
    memset(buf,0,1024);
    //recv(sockfd,buf,N,0);
    while (1)
    {
        num = recv(sockfd,buf,1024,0);
        if(0 == strcmp("FINISHED",buf))
        {
            fclose(fd1);
            printf("完成传输！\n");
            for(;;);
            break;
        }
        fwrite(buf,sizeof(char),num,fd1);
        memset(buf,0,1024);
        send(sockfd,"OK",N,0);
    }
    //pthread_mutex_unlock(&mutex);
    pthread_cond_signal(&cond);
    free(buf);
    return;
}