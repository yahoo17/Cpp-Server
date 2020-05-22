#include <thread>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <sys/types.h>
int main()
{
    int sockfd=socket(AF_INET,SOCK_STREAM,0);
    struct  sockaddr_in servaddr;
    servaddr.sin_family=AF_INET;
    servaddr.sin_port=6666;
    inet_pton(AF_INET,"127.0.0.1",&servaddr.sin_addr);
    connect(sockfd,(struct sockaddr*) &servaddr,sizeof(servaddr));
    char buf[100]="kkkk";
    send(sockfd,buf,sizeof(buf),0);
}