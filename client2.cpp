#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main( int count ,char ** argv)
{
    //socket
    //connect
    //write
    //close
    int clientSocket;
    clientSocket=socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in clientAddr;
    clientAddr.sin_family=AF_INET;
    clientAddr.sin_port=6660;
  
    connect(clientSocket,(struct sockaddr *)&clientAddr,sizeof(clientAddr));
    char buff[4096];
    fgets(buff,4096,stdin);
    if(send(clientSocket,buff,strlen(buff),0))
    {

    }
    close(clientSocket);
    return 0;
}