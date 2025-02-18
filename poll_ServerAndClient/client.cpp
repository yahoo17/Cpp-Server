#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
#include <poll.h>

#define MAXLINE 4096

static void handle_connection(int sockfd)
{
    char sendline[MAXLINE];
    char recvline[MAXLINE];
    int maxfdp;
    int stdineof;
    struct pollfd pfds[2];
    int n;
    //添加连接描述符
    pfds[0].fd=sockfd;
    pfds[0].events=POLLIN;
    
    pfds[1].fd=STDIN_FILENO;
    pfds[1].events=POLLIN;
    while (1)
    {
        poll(pfds,2,-1);
        if(pfds[0].revents&POLLIN)
        {
            n=read(sockfd,recvline,MAXLINE);
            if(n==0)
            {
                fprintf(stderr,"cient : server is closed.\n");
                close(sockfd);
            }
        }

        if(pfds[1].revents&POLLIN)
        {
            n=read(STDIN_FILENO,sendline,MAXLINE);
            if(n==0){
                shutdown(sockfd,SHUT_WR);
                continue;
            }
            write(sockfd,sendline,n);
        }
    }
}
int main( int argc ,char * argv[])
{
    int connfd=0;
    int cLen=0;
    struct sockaddr_in client;
    if(argc<2)
    {
        printf( "Usage: clientent[server IP address] \n");
        return -1;
    }
    client.sin_family=AF_INET;
    client.sin_port=6666;
    client.sin_addr.s_addr=inet_addr (argv[1]);
    connfd=socket(AF_INET,SOCK_STREAM,0);
    if(connfd<0)
    {
        perror("socket \n");
        return -1;

    }
    if(connect(connfd , (struct sockaddr * )&client,sizeof(client))<0)
    {
        perror("connect\n");
        return -2;
    }
    //handle 
    handle_connection(connfd);
    return 0;
}