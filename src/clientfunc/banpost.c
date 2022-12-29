#include "sockcreate.h"
#include "myclient.h"
void Ban_to_psot(int sockfd)
{
    char buf[N] = {0};
    pthread_cond_wait(&cond,&mutex);
    printf("输入要禁言的用户ID\n");
    scanf("%s",buf);
    send(sockfd,buf,N,0);
    return;
}