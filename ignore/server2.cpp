#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
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
int main()
{
    //socket
    //bind
    //listen
    //accept
    //recv
    //print
    int listenfd;


   if((listenfd=socket(AF_INET,SOCK_STREAM,0))==-1) 
   {
       //error
   }
    //三个参数,第一个是domain address_family, 第二个是type 字节流套接字
    //,第三个参数是协议 0就是自动选择

    int connectfd;

    struct  sockaddr_in serverAddr;

    memset(&serverAddr,0,sizeof(serverAddr));
    serverAddr.sin_addr.s_addr=htons(INADDR_ANY);
    serverAddr.sin_port=6660;
    serverAddr.sin_family=AF_INET;

    if(bind(listenfd,(struct sockaddr *)&serverAddr,sizeof(serverAddr ))==-1)
    {
        //error
    }

    if(listen(listenfd,1000)==-1)
    {
        //error
    }
    //listen(fd,最大连接数)

    
    char buff[4096];
    int n;
    while(1)
    {
        if((connectfd=accept(listenfd,(struct sockaddr*)NULL,NULL))==-1)
        {
                printf("accept socket error");
                continue;
        }
        n=recv(connectfd,buff,4096,0);
        buff[n]='\0';
        printf( " message is %s \n",buff);

    }
    close(listenfd);
    return 0;

}
