#include <stdio.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include  <errno.h>
#include <string.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include  <iostream>
#define LISTEN_PORT 6666
//优势在于, 不用创建进程 线程 系统开销小
int main()
{
    int serverfd,acceptfd;
    //监听socket:serverfd
    //监听数据传输socket:acceptfd
    struct sockaddr_in my_addr; //本机地址信息
    struct sockaddr_in their_addr;//客户地址信息
    
    unsigned int sin_size,myport=6666 ,listennum=10;

    if((serverfd =socket(AF_INET,SOCK_STREAM,0))==-1)
    {
        perror("socket");
        return -1;
        //创建服务器监听socket
    }
    printf(" socket ok \n");


    memset(&my_addr,0,sizeof(my_addr));
    my_addr.sin_family=AF_INET;
    my_addr.sin_port=htons(6666);
    my_addr.sin_addr.s_addr=htons(INADDR_ANY);
    bzero(&(my_addr.sin_zero),0);
    //绑定本机的端口号到监听的serverfd 这个socket
    if(bind(serverfd,(struct sockaddr *)&my_addr,sizeof(my_addr))==-1)
    {
        printf("bind error: %s (errno: %d) \n",   strerror(errno),errno           );
        return 0;
    }


    printf("bind  ok\n");
    if(listen(serverfd,listennum)==-1)
    {
        printf("listen socket error\n");
    }
    printf("listen ok\n");
    printf("_______wait for client____\n");

    fd_set client_fdset;        //监控文件描述符集合
    int maxsock;//监控文件描述符的最大文件号
    struct  timeval tv;//超时返回时间
    int client_sockfd[5];//存放活动的sockfd
    bzero( (void*)client_sockfd,sizeof(client_sockfd));
    int conn_amout=0;//用来记录描述符数量
    maxsock=serverfd;
    int ret=0;

    int n;
    char buffer[4096];
    const int MAXLINE=4096;
    while(1)
    {
        //初始化文件描述符到集合
        FD_ZERO (&client_fdset);
        //加入服务器描述符
        FD_SET(serverfd,&client_fdset);
        //设置超时时间为30s
        tv.tv_sec=30;
        tv.tv_usec=0;
        for(int i=0;i<5;++i)
        //程序中listen的参数设为5 所以i必须小于5
        {
            if(client_sockfd[i]!=0)
            {
                ////把活动的句柄加入到文件描述符里面
                FD_SET(client_sockfd[i],&client_fdset);
                printf("  put socket %i in fdset\n",i);
            }
        }
        /*
        #include <sys/select.h>
        #include <sys/time.h>
        select的函数原型int select(int maxfdp1,fd_set *readset,fd_set  *writeset,fd_set * except_set,const struct timeval * timeout)
        有就绪的描述符就返回数目 超时就0 出错就-1
        */
        printf("//__select函数被调用__\n");
        ret=select(maxsock+1,&client_fdset,NULL,NULL,&tv);
        if(ret<0)
        {
                perror("select  error\n");
                break;
        }
        else if(ret==0)
        {
                printf(" time out \n");
                continue;
        }


        //轮询各个文件描述符
        int  decount=0;
      
        for(int i=0;i<conn_amout;++i)
        {
                
                //FD_ISSET检查client_socket是否可以读写 >0表示可以读写
                if(FD_ISSET(client_sockfd[i],&client_fdset))
                {
                    
                    printf(" start  recv from client[%d]: \n",i);                   
                    ret=recv(client_sockfd[i],buffer,1024,0);
                    if(ret<=0)
                    {
                        printf("client[%d] close \n ",i);
                        if(client_sockfd[i]>=maxsock)

                        close(client_sockfd[i]);

                        decount++;
                        FD_CLR(client_sockfd[i],&client_fdset  );
                        client_sockfd[i]=0;
                    }
                    else
                    {
                        maxsock=std::max(client_sockfd[i],maxsock);
                        printf(" recv from client [%d]: %s \n",i,buffer);
                    }
                }
        }
        conn_amout-=decount;
            //检查是否有新的连接,如果有,我们就接受连接,加入到client_sockfd中
                if(FD_ISSET(serverfd,&client_fdset))
                {
                        // accept connection
                        struct sockaddr_in client_addr_in;
                        
                        size_t size=  sizeof(struct sockaddr_in);

                        int  sock_client=accept(serverfd,(struct sockaddr*)(&client_addr_in),(unsigned int *)(& size));

                        if(sock_client<0)
                        {
                            perror(" accept error \n");
                            continue;
                        }

                        //把连接加入到文件描述符集合中
                        if( conn_amout<5)
                        {
                            printf("new connection client[%d] %s : %d \n",conn_amout,inet_ntoa(client_addr_in.sin_addr),ntohs(client_addr_in.sin_port));
                            bzero(buffer,sizeof(buffer));
                            //活跃的client_socket[conn_amout]号进程
                            client_sockfd[conn_amout]=sock_client;
                            conn_amout++;
                            bzero(buffer,1024);
                            strcpy(buffer,"this is server! welcome ! \n");
                            send(sock_client,buffer,1024,0);

                           
                            ret=recv(sock_client,buffer,1024,0);
                            if(ret<0)
                            {
                                perror("recv error ! \n");
                                close(serverfd);
                                return -1;

                            }
                            printf(" 收到消息 : %s \n",buffer);
                            if(sock_client>=maxsock)
                                maxsock=sock_client;
                           
                            
                        }


                }


        }//这个括号是while循环的括号

    
    
    
    
    
    
    
    for(int i =0 ;i<5;++i)
    {
        if(client_sockfd[i]!=0)
        {
            close(client_sockfd[i]);
        }
    }
    close(serverfd);
    return 0;



  
    
}