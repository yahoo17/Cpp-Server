#include <netinet/in.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <time.h>
#include <sys/types.h>
#include<sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#define SERV_PORT 6666
//定义服务器端口号
#define IPADDRESS "127.0.0.1"

#define EPOLLEVENTS 20
#define MAXSIZE 1024

#define FDSIZE 1024
void modify_event(int epollfd,int fd,int state);
void delete_event(int epollfd,int fd,int state);
void add_event(int epollfd,int fd,int state);
void do_write(int epollfd,int fd,int sockfd,char *buf);
void do_read(int epollfd,int fd,int sockfd,char *buf);
void handle_events(int epollfd,struct epoll_event * events,int num,int sockfd,char *buf);
void handle_connection(int sockfd);

int count=0;
//全局变量
int main(int argc,char ** argv)
{
    int sockfd;
    struct sockaddr_in serveraddr;
    sockfd=socket(AF_INET,SOCK_STREAM,0);
    bzero(&serveraddr,sizeof(serveraddr));
    serveraddr.sin_family=AF_INET;
    serveraddr.sin_port=htons(SERV_PORT);
    inet_pton(AF_INET,IPADDRESS,&serveraddr.sin_addr);
    connect(sockfd,(struct sockaddr *)&serveraddr,sizeof(serveraddr));
    //handle连接
    handle_connection(sockfd);
    close(sockfd);
    return 0;




}

void modify_event(int epollfd,int fd,int state)
{
    struct epoll_event ev;
    ev.events=state;
    ev.data.fd=fd;
    epoll_ctl(epollfd,EPOLL_CTL_MOD,fd,&ev);
}
void delete_event(int epollfd,int fd,int state)
{
    struct epoll_event ev;
    ev.data.fd=fd;
    ev.events=state;
    epoll_ctl(epollfd,EPOLL_CTL_DEL,fd,&ev);

}
void add_event(int epollfd,int fd,int state)
{
    struct epoll_event ev;
    ev.events=state;
    ev.data.fd=fd;
    epoll_ctl(epollfd,EPOLL_CTL_ADD,fd,&ev);
}
void do_write(int epollfd,int fd,int sockfd,char *buf)
{
    int nwrite;
    char temp[100];
    buf[strlen(buf)-1]='\0';
    snprintf(temp,sizeof(temp),"%s_%02d\n",buf,count++);
    nwrite=write(fd,temp,MAXSIZE);
    if(nwrite==-1)
    {
        perror("write error");
    }
    else
    {
        if(fd==STDIN_FILENO)
            delete_event(epollfd,fd,EPOLLOUT);
        else
        {
            
            modify_event(epollfd,fd,EPOLLIN);
        }
        
    }
    memset(buf,0,MAXSIZE);
    
}
void do_read(int epollfd,int fd,int sockfd,char *buf)
{
    int nread;
    nread=read(fd,buf,MAXSIZE);
    if(nread==-1)
    {
        perror("read error:");
        
    }
    else if(nread==0)
    {
        fprintf(stderr,"server close\n");


    }
    else
    {
        //success
        if(fd==STDIN_FILENO)
            add_event(epollfd,sockfd,EPOLLOUT);
        else
        {
            delete_event(epollfd,sockfd,EPOLLIN);
            add_event(epollfd,sockfd,EPOLLOUT);
        }
        
    }
    
}
void handle_events(int epollfd,struct epoll_event * events,int num,int sockfd,char *buf)
{
    int fd;
    int i;
    for(i=0;i<num;i++)
    {
        fd=events[i].data.fd;
        if(events[i].events&EPOLLIN)
        {
            do_read(epollfd,fd,sockfd,buf);
        }
        else if(events[i].events&EPOLLOUT)
        {
            do_write(epollfd,fd,sockfd,buf);
        }
    }



}
void handle_connection(int sockfd)
{
    int epollfd;
    struct epoll_event events[EPOLLEVENTS];
    char buf[MAXSIZE];
    int ret ;
    epollfd=epoll_create(FDSIZE);
    add_event(epollfd,STDIN_FILENO,EPOLLIN);
    while (1)
    {
        ret=epoll_wait(epollfd,events,EPOLLEVENTS,-1);
        handle_events(epollfd,events,ret,sockfd,buf);
        /* code */
    }
    
}
