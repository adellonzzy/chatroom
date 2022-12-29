#include "sockcreate.h"
#include "myclient.h"
void Quit_and_Save(int sockfd,const char *id,void *arg)
{
    //printf("success\n");
    int * p = (int *)arg;
    *p = END_CHAT;
    pthread_cond_wait(&cond,&mutex);

    char buf[N] = {0};
    char addr[N] = {0};
    sprintf(addr,"./../../usr/%s.txt",id) ;
    FILE *fp = fopen(addr,"w+");
    if(NULL == fp)
    {
        perror("fopen");
        return;
    }
    while(1)
    {
        
        printf("success1\n");
        //memset(buf,0,N);
        recv(sockfd,buf,N,0);
        printf("success8\n");

        if (0 == strcmp("QUIT",buf))//服务器发来的提示不保存
        {
            break;
        }
        if (0 == strcmp("finished",buf))
        {
            break;
        }
        fprintf(fp,"%s",buf);
        memset(buf,0,N);
    }
    //printf("finished\n");
    fclose(fp);
    return;
}
