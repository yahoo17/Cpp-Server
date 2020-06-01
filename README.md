# Server
 目录结构

//Server

​	//thread_pool

​		//thread_pool.h 	线程池代码

​		//README.md  	对线程池的介绍

​	//PythonServer  demo玩意

​	//lock

​		//locker.h  实现RAII机制,对互斥锁,信号量,条件变量进行封装

​	//epoll_serverandclient

​		//epollclient.cpp

​		//server.cpp

​	//select_serverandclient

​		//selectclient.cpp

​		//server.cpp

​	//poll_serverandclient

​		//pollclient.cpp

​		//server.cpp



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

//2020/5/9

用epoll写了客户端



//2020/5/10

1.Python写了基于WCGI的服务器

2.Python用flask直接返回页面

3.Python用flask和 template返回html页面

<<<<<<< HEAD


//2020/5/18

1.写了poll的服务器

2.写了poll的客户端

=======
//2020/5/18

1.C++ 写了poll的服务器

2.C++写了poll的客户端

//2020/5-25~ 2020/5/31

1.学习了python语法

2.学习了django框架

3.学习了flask框架

4.写了django和flask的demo

//2020/6/1

1.封装信号量,互斥锁,条件变量,实现RAII机制

2.C++实现线程池,并在内部加入请求队列,用互斥锁保护请求队列,用信号量通知任务处理

3.为数据库连接池留了接口

