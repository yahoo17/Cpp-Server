
#include <netinet/in.h>
#include <sys/socket.h>
#include <errno.h>
#include <stdlib.h>
#include<unistd.h>
#include <stdio.h>
#include <string.h>
#define DEFAULT_PORT 6666
int main(int argc,char* argv[])
{
    int connfd=0;
    int cLen=0;
    struct sockaddr_in  client;
    if(argc<2)
    {
        printf("Usage :clientent [server IP address]\n");

        return -1 ;
    }
    client.sin_family=AF_INET;
    client.sin_port=htons(DEFAULT_PORT);
    client.sin_addr.s_addr=(AF_INET,SOCK_STREAM,0);
    connfd=socket(AF_INET,SOCK_STREAM,0);
    if(connfd<0)
    {
        perror("socker");
        return -1;
    }
    if(connect(connfd,(struct sockaddr* )&client,sizeof(client))<0)
    {
        perror(" connect error");
        return -1;
    }
    char buffer[1024];
    bzero(buffer,sizeof(buffer));
    recv(connfd,buffer,1024,0);
    printf(buffer,sizeof(buffer));

    bzero(buffer,sizeof(buffer  ));
    strcpy(buffer,"this is client\n");
    send(connfd,buffer,1024,0);
    while(1)
    {
            bzero(buffer,sizeof(buffer));
            scanf("%s",buffer);
            int p=strlen(buffer);
            buffer[p]='\0';
            send(connfd,buffer,1024,0);
            printf(" i have send buffer\n");

    }
    close(connfd);
    return 0;
}