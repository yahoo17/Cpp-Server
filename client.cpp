#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <arpa/inet.h>
int main(int argcount ,char ** arg_variables)
{
    int socketfd;
    struct  sockaddr_in serveraddr;

    if(argcount!=2)
    {
        printf("用法错误,正确的用法是 ./client <ipaddress> \n");
        return 0;
    }
    if((socketfd=socket(AF_INET,SOCK_STREAM,0))<0)
    {
        printf("error");
        return 0;
    }
    memset(&serveraddr,0,sizeof(serveraddr));
    serveraddr.sin_family=AF_INET;
    serveraddr.sin_port=htons(6776);
    
    if(   (inet_pton(AF_INET,arg_variables[1],&serveraddr.sin_addr))<=0)
    {
      printf( "inet_pton error for %s  \n",arg_variables[1]);
      return 0;
    }

    if(  (connect (socketfd ,(struct  sockaddr*)&serveraddr, sizeof(serveraddr)  ))<0)
    {
            printf( " connect error: %s( errno: %d)\n",strerror(errno),errno);
            return 0;
    }   

    printf( "send message to server\n");
    char recvline[4096];
    char sendline[4096];
    fgets(sendline,4096,stdin);
    if(send(socketfd,sendline,strlen(sendline),0)<0 )
    {
        printf(" send error \n");
        return 0;
    }
    close(socketfd);
    return 0;
 
}