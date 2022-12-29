#include "sockcreate.h"
#include "myserver.h"
#include "sqlite_op.h"

void transfile(int acceptfd)
{
    char *buf = (char *)malloc(sizeof(char)*1024);
    memset(buf,0,1024);
    //char buf[1024] = {0};
    recv(acceptfd,buf,N,0);
    send(acceptfd,"OK",N,0);
    FILE *fd = fopen(buf,"r");
    if(NULL == fd)
    {
        perror("fopen");
        return ;
    }
    memset(buf,0,N);
    int num;
    while (1)
    {
        num = fread(buf,sizeof(char),1024,fd);
        printf("num = %d\n",num);
        send(acceptfd,buf,num,0);
        memset(buf,0,1024);
        if(feof(fd))
        {
            send(acceptfd,"FINISHED",N,0);
            fclose(fd);
            break;
        }   
        if(recv(acceptfd,buf,N,0))
        {
            memset(buf,0,1024);
            continue;
        }
    }
    // sleep(1);
    // memset(buf,0,1024);
    // send(acceptfd,"FINISHED",N,0);
    // fclose(fd);
    free(buf);
    return ;
}