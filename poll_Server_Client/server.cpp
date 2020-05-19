//poll
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



#define  LISTENPORT 6666
#define MAXSIZE 4096
#define LISTENCONNECTCOUNT 10
#define OPEN_MAX 1000
#define INFTIM -1

int bind_and_listen()
{
    int serverfd;
    //一.监听socket 就是serverfd
    serverfd=socket(AF_INET,SOCK_STREAM,0);
    if(serverfd==-1)
    {
        perror("socket");
        return -1;       
    }
    printf("socket OK\n");

    //二.本机地址
    struct sockaddr_in addr;
    addr.sin_addr.s_addr=INADDR_ANY;
    addr.sin_family=AF_INET;
    addr.sin_port=LISTENPORT;
    
    //三.绑定
    if (bind(serverfd,(struct sockaddr *) &addr,sizeof(struct sockaddr))==-1)
    {
        perror("bind error");
        return -2;
    }
    printf("bind ok\n");

    //四.监听
    if(listen(serverfd,LISTENCONNECTCOUNT)==-1)
    {
        perror("listen errot");
        return -3;

    }
    printf(" listen ok \n");
    return serverfd;
}

//IO多路复用poll
void do_poll(int listenfd)
{
    int connfd,sockfd;
    struct sockaddr_in cliaddr;
    socklen_t cliaddrlen;
    struct pollfd clientfds[LISTENCONNECTCOUNT];
    int maxi;
    int i;
    int nready;
    //添加监听描述符
    clientfds[0].fd=listenfd;
    clientfds[0].events=POLLIN;
    //初始化-1
    for(int i=1;i<OPEN_MAX;i++)
    {
        clientfds[i].fd=-1;

    }
    maxi=0;
    
    while (1)
    {
        //获取当前可用的描述符的个数
        nready=poll(clientfds,maxi+1,INFTIM);
        if(nready==-1)
        {
            perror("poll error\n");
            exit(1);

        }
        //测试监听描述符师傅已经准备好了
        if(clientfds[0].revents&POLLIN)
        {
                cliaddrlen=sizeof(cliaddr);

        //接受新的连接
        if(   (connfd=accept(listenfd,(struct sockaddr*)&cliaddr,&cliaddrlen))==-1)
        {
                if(errno ==EINTR)
                {
                    continue;
                }
                else
                {
                    perror("accept error:");
                    exit(1);
                }
                
        }
        fprintf(stdout,"accept a new client: %s:%d \n",inet_ntoa(cliaddr.sin_addr),cliaddr.sin_port);
        
        //将新的连接描述符加入到数组中
        for(i=1;i<OPEN_MAX;i++)
        {
            if(clientfds[i].fd<0)
            {
                clientfds[i].fd=connfd;
                break;
            }
        }  
            if(i==OPEN_MAX)
            {
                fprintf(stderr,"too many clients.\n");
                exit(1);
            }
            //将新的描述符添加到该描述符集合中
            clientfds[i].events=POLLIN;

            //记录客户连接套接字的个数
            maxi=(i>maxi?i:maxi);
            if(--nready<=0)
                continue;
            
        }

        //处理多个连接上客户端发来的包
        char buf[MAXSIZE];
        memset(buf,0,MAXSIZE);
        int readlen=0;
        for(i=1;i<=maxi;i++)
        {


            if(clientfds[i].fd<0)
            {
                continue;
            }

            if(clientfds[i].revents&POLLIN)
            {

                //接受客户端的消息
                readlen=read(clientfds[i].fd,buf,MAXSIZE);
                if(readlen==0)
                {
                    close(clientfds[i].fd);
                    clientfds[i].fd=-1;
                    continue;

                }
                write(STDOUT_FILENO,buf,readlen);

                write(clientfds[i].fd,buf,readlen);




       

        }

    }
    
    }
}







int main(int argc,char * argv[])
{

    int listenfd=bind_and_listen();
    if(listenfd<0)
    {
            return 0;
    }
    do_poll(listenfd);
    return 0;
}