# Server
 我的服务器,慢慢写

在linux下面写,但是不想装git,所以一段时间更新一次

//2020/5/6

阻塞I/O 传统经典 

>
>
>服务器 socket bind listen accept recv  sockaddr_in #include<sys.socket.h>  <netinet/in.h>
>
>客户端  socket connect send
>
>

//2020/5/6

其实已经写好了select I/O多路复用 ,并且优化了连接 客户端可以随意退出 并让位给其他客户端 测试最大连接为5时 最大体会就是maxsock天天要更新 