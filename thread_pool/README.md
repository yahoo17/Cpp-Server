### 服务器编程基本框架

主要由I/O单元，逻辑单元和网络存储单元组成，其中每个单元之间通过请求队列进行通信，从而协同完成任务。

其中I/O单元用于处理客户端连接，读写网络数据；逻辑单元用于处理业务逻辑的线程；网络存储单元指本地数据库和文件等。

![img](https://mmbiz.qpic.cn/mmbiz_jpg/6OkibcrXVmBH2ZO50WrURwTiaNKTH7tCia3QENT2MZibwxf74GnEpuxqz30UHUJZuJ9d0vYP8SzkU5fK1sMw4DNzng/640?wx_fmt=jpeg&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)

### 五种I/O模型

- **阻塞IO**:调用者调用了某个函数，等待这个函数返回，期间什么也不做，不停的去检查这个函数有没有返回，必须等这个函数返回才能进行下一步动作
- **非阻塞IO**:非阻塞等待，每隔一段时间就去检测IO事件是否就绪。没有就绪就可以做其他事。非阻塞I/O执行系统调用总是立即返回，不管时间是否已经发生，若时间没有发生，则返回-1，此时可以根据errno区分这两种情况，对于accept，recv和send，事件未发生时，errno通常被设置成eagain
- **信号驱动IO**:linux用套接口进行信号驱动IO，安装一个信号处理函数，进程继续运行并不阻塞，当IO时间就绪，进程收到SIGIO信号。然后处理IO事件。
- **IO复用**:linux用select/poll函数实现IO复用模型，这两个函数也会使进程阻塞，但是和阻塞IO所不同的是这两个函数可以同时阻塞多个IO操作。而且可以同时对多个读操作、写操作的IO函数进行检测。知道有数据可读或可写时，才真正调用IO操作函数
- **异步IO**:linux中，可以调用aio_read函数告诉内核描述字缓冲区指针和缓冲区的大小、文件偏移及通知的方式，然后立即返回，当内核将数据拷贝到缓冲区后，再通知应用程序。

**注意：阻塞I/O，非阻塞I/O，信号驱动I/O和I/O复用都是同步I/O。同步I/O指内核向应用程序通知的是就绪事件，比如只通知有客户端连接，要求用户代码自行执行I/O操作，异步I/O是指内核向应用程序通知的是完成事件，比如读取客户端的数据后才通知应用程序，由内核完成I/O操作。**

### 事件处理模式

- reactor模式中，主线程(**I/O处理单元**)只负责监听文件描述符上是否有事件发生，有的话立即通知工作线程(**逻辑单元** )，读写数据、接受新连接及处理客户请求均在工作线程中完成。通常由**同步I/O**实现。
- proactor模式中，主线程和内核负责处理读写数据、接受新连接等I/O操作，工作线程仅负责业务逻辑，如处理客户请求。通常由**异步I/O**实现。

### 同步I/O模拟proactor模式

由于异步I/O并不成熟，实际中使用较少，这里将使用同步I/O模拟实现proactor模式。

同步I/O模型的工作流程如下（epoll_wait为例）：

> - 主线程往epoll内核事件表注册socket上的读就绪事件。
> - 主线程调用epoll_wait等待socket上有数据可读
> - 当socket上有数据可读，epoll_wait通知主线程,主线程从socket循环读取数据，直到没有更多数据可读，然后将读取到的数据封装成一个请求对象并插入请求队列。
> - 睡眠在请求队列上某个工作线程被唤醒，它获得请求对象并处理客户请求，然后往epoll内核事件表中注册该socket上的写就绪事件
> - 主线程调用epoll_wait等待socket可写。
> - 当socket上有数据可写，epoll_wait通知主线程。主线程往socket上写入服务器处理客户请求的结果。

### 并发编程模式

并发编程方法的实现有多线程和多进程两种，但这里涉及的并发模式指I/O处理单元与逻辑单元的协同完成任务的方法。

- 半同步/半异步模式
- 领导者/追随者模式

### 半同步/半反应堆

半同步/半反应堆并发模式是半同步/半异步的变体，将半异步具体化为某种事件处理模式.

并发模式中的同步和异步

> - 同步指的是程序完全按照代码序列的顺序执行
> - 异步指的是程序的执行需要由系统事件驱动

半同步/半异步模式工作流程

> - 同步线程用于处理客户逻辑
> - 异步线程用于处理I/O事件
> - 异步线程监听到客户请求后，就将其封装成请求对象并插入请求队列中
> - 请求队列将通知某个工作在**同步模式的工作线程**来读取并处理该请求对象

半同步/半反应堆工作流程（以Proactor模式为例）

> - 主线程充当异步线程，负责监听所有socket上的事件
> - 若有新请求到来，主线程接收之以得到新的连接socket，然后往epoll内核事件表中注册该socket上的读写事件
> - 如果连接socket上有读写事件发生，主线程从socket上接收数据，并将数据封装成请求对象插入到请求队列中
> - 所有工作线程睡眠在请求队列上，当有任务到来时，通过竞争（如互斥锁）获得任务的接管权

### 线程池

- 空间换时间,浪费服务器的硬件资源,换取运行效率.
- 池是一组资源的集合,这组资源在服务器启动之初就被完全创建好并初始化,这称为静态资源.
- 当服务器进入正式运行阶段,开始处理客户请求的时候,如果它需要相关的资源,可以直接从池中获取,无需动态分配.
- 当服务器处理完一个客户连接后,可以把相关的资源放回池中,无需执行系统调用释放资源.





### 

#### **静态成员变量**

将类成员变量声明为static，则为静态成员变量，与一般的成员变量不同，无论建立多少对象，都只有一个静态成员变量的拷贝，静态成员变量属于一个类，所有对象共享。

静态变量在编译阶段就分配了空间，对象还没创建时就已经分配了空间，放到全局静态区。

- 静态成员变量

- - 最好是类内声明，类外初始化（以免类名访问静态成员访问不到）。
  - 无论公有，私有，静态成员都可以在类外定义，但私有成员仍有访问权限。
  - 非静态成员类外不能初始化。
  - 静态成员数据是共享的。

#### **静态成员函数**

将类成员函数声明为static，则为静态成员函数。

- 静态成员函数

- - 静态成员函数可以直接访问静态成员变量，不能直接访问普通成员变量，但可以通过参数传递的方式访问。
  - 普通成员函数可以访问普通成员变量，也可以访问静态成员变量。
  - 静态成员函数没有this指针。非静态数据成员为对象单独维护，但静态成员函数为共享函数，无法区分是哪个对象，因此不能直接访问普通变量成员，也没有this指针。

#### **pthread_create陷阱**

首先看一下该函数的函数原型。

```
1#include <pthread.h>2int pthread_create (pthread_t *thread_tid,                 //返回新生成的线程的id3                    const pthread_attr_t *attr,         //指向线程属性的指针,通常设置为NULL4                    void * (*start_routine) (void *),   //处理线程函数的地址5                    void *arg);                         //start_routine()中的参数
```

函数原型中的第三个参数，为函数指针，指向处理线程函数的地址。该函数，要求为静态函数。如果处理线程函数为类成员函数时，需要将其设置为**静态成员函数**。

#### **this指针的锅**

pthread_create的函数原型中第三个参数的类型为函数指针，指向的线程处理函数参数类型为`(void *)`,若线程函数为类成员函数，则this指针会作为默认的参数被传进函数中，从而和线程函数参数`(void*)`不能匹配，不能通过编译。

静态成员函数就没有这个问题，里面没有this指针。

### 线程池分析

线程池的设计模式为半同步/半反应堆，其中反应堆具体为Proactor事件处理模式。

具体的，主线程为异步线程，负责监听文件描述符，接收socket新连接，若当前监听的socket发生了读写事件，然后将任务插入到请求队列。工作线程从请求队列中取出任务，完成读写数据的处理。

#### **线程池类定义**

具体定义可以看代码。需要注意，线程处理函数和运行函数设置为私有属性。

```C++
template<typename T>
 2class threadpool{
 3    public:
 4        //thread_number是线程池中线程的数量
 5        //max_requests是请求队列中最多允许的、等待处理的请求的数量
 6        //connPool是数据库连接池指针
 7        threadpool(connection_pool *connPool, int thread_number = 8, int max_request = 10000);
 8        ~threadpool();
 9
10        //像请求队列中插入任务请求
11        bool append(T* request);
12
13    private:
14        //工作线程运行的函数
15        //它不断从工作队列中取出任务并执行之
16        static void *worker(void *arg);
17
18        void run();
19
20    private:
21        //线程池中的线程数
22        int m_thread_number;
23
24        //请求队列中允许的最大请求数
25        int m_max_requests;
26
27        //描述线程池的数组，其大小为m_thread_number
28        pthread_t *m_threads;
29
30        //请求队列
31        std::list<T *>m_workqueue;    
32
33        //保护请求队列的互斥锁    
34        locker m_queuelocker;
35
36        //是否有任务需要处理
37        sem m_queuestat;
38
39        //是否结束线程
40        bool m_stop;
41
42        //数据库连接池
43        connection_pool *m_connPool;  
44};
```



#### **线程池创建与回收**

构造函数中创建线程池,pthread_create函数中将类的对象作为参数传递给静态函数(worker),在静态函数中引用这个对象,并调用其动态方法(run)。

具体的，类对象传递时用this指针，传递给静态函数后，将其转换为线程池类，并调用私有成员函数run。

 ```
1template<typename T>
 2threadpool<T>::threadpool( connection_pool *connPool, int thread_number, int max_requests) : m_thread_number(thread_number), m_max_requests(max_requests), m_stop(false), m_threads(NULL),m_connPool(connPool){
 3
 4    if(thread_number<=0||max_requests<=0)
 5        throw std::exception();
 6
 7    //线程id初始化
 8    m_threads=new pthread_t[m_thread_number];
 9    if(!m_threads)
10        throw std::exception();
11    for(int i=0;i<thread_number;++i)
12    {
13        //循环创建线程，并将工作线程按要求进行运行
14        if(pthread_create(m_threads+i,NULL,worker,this)!=0){
15            delete [] m_threads;
16            throw std::exception();
17        }
18
19        //将线程进行分离后，不用单独对工作线程进行回收
20        if(pthread_detach(m_threads[i])){
21            delete[] m_threads;
22            throw std::exception();
23        }
24    }
25}
 ```



#### **向请求队列中添加任务**

通过list容器创建请求队列，向队列中添加时，通过互斥锁保证线程安全，添加完成后通过信号量提醒有任务要处理，最后注意线程同步。

```
 1template<typename T>
 2bool threadpool<T>::append(T* request)
 3{
 4    m_queuelocker.lock();
 5
 6    //根据硬件，预先设置请求队列的最大值
 7    if(m_workqueue.size()>m_max_requests)
 8    {
 9        m_queuelocker.unlock();
10        return false;
11    }
12
13    //添加任务
14    m_workqueue.push_back(request);
15    m_queuelocker.unlock();
16
17    //信号量提醒有任务要处理
18    m_queuestat.post();
19    return true;
20}
```



#### **线程处理函数**

内部访问私有成员函数run，完成线程处理要求。

```
1template<typename T>
2void* threadpool<T>::worker(void* arg){
3
4    //将参数强转为线程池类，调用成员方法
5    threadpool* pool=(threadpool*)arg;
6    pool->run();
7    return pool;
8}
```



#### **run执行任务**

主要实现，工作线程从请求队列中取出某个任务进行处理，注意线程同步。

```
 1template<typename T>
 2void threadpool<T>::run()
 3{
 4    while(!m_stop)
 5    {    
 6        //信号量等待
 7        m_queuestat.wait();
 8
 9        //被唤醒后先加互斥锁
10        m_queuelocker.lock();
11        if(m_workqueue.empty())
12        {
13            m_queuelocker.unlock();
14            continue;
15        }
16
17        //从请求队列中取出第一个任务
18        //将任务从请求队列删除
19        T* request=m_workqueue.front();
20        m_workqueue.pop_front();
21        m_queuelocker.unlock();
22        if(!request)
23            continue;
24
25        //从连接池中取出一个数据库连接
26        request->mysql = m_connPool->GetConnection();
27
28        //process(模板类中的方法,这里是http类)进行处理
29        request->process();
30
31        //将数据库连接放回连接池
32        m_connPool->ReleaseConnection(request->mysql);
33    }
34}
```

