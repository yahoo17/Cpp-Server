#include <sys/epoll.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string>
int epollfd;
void epoll_init();
int listen_bind(string ipaddress,int port,int listenNum)
{
    int listensocket=socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in listenaddr_in;
    listenaddr_in.sin_addr.s_addr=INADDR_ANY;

    listenaddr_in.sin_port=port;
    bind(listensocket,(struct sockaddr *)&listenaddr_in,0);
    listen(listen_bind,listenNum);
    return listensocket;
}
int main()
{
    
    int listenfd=listen_bind("127.0.0.1",6666);
    epoll_init();
    epoll_add(listensocket);


}
void epoll_init()
{
    epollfd=epoll_create(100);
   
}
void epoll_add(int fileDesc)
{
    struct epoll_event event_temp;
    event_temp.data.fd=fileDesc;
    event_temp.events=EPOLLIN;
    epoll_ctl(epollfd,EPOLL_CTL_ADD,fileDesc,&event_temp);
}

