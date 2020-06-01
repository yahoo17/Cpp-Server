#ifndef THREAD_POOL
#define THREAD_POOL
#include <exception>
#include <pthread.h>
#include <stdio.h>
#include <list>
class connection_pool;
template<typename T>
class  thread_pool
{
private:
	int m_thread_number;
    	int m_max_request;
   	int m_thread_poll_size;
    	pthread_t *m_threads;
	bool m_stop;
   	connection_pool m_conn_pool;
    	std::list<T*> m_workQueue;
	sem m_queuestat; 				//是否有任务需要处理
	locker  m_queuelock;				//保护请求队列的互斥锁
private:
	//工作线程运行的函数, 它不断地从工作队列里取出任务来执行
	static void * worker(void * arg);
	void run();
public:
    //connPool是数据库连接池的指针
    // thread_pool是线程池里线程的数量
    //max_request是处理的最大请求数
    	 thread_pool(connection_pool * connPool,int thread_number=8,int max_request=10000);
	thread_pool(const thread_pool & other)=deleted;
	thread_pool operator= (const thread_pool & other)=deleted;
	 ~ thread_pool();
	bool append(T * request)

};

template<typename T>
void thread_pool<T>::run()
{
	while(!m_stop)
	{
		m_queuestat.wait();
		m_queuelock.lock();

		if(m_workQueue.empty())
		{

		}
		T* request=m_workQueue.front();
		m_workQueue.pop_front();
		m_queuelocker.unlock();
		if(!request)
			continue;
		connectionRAII mysqlcon(&request->mysql,m_connPool);
		request->process();


	}
}
template<typename T>
void * thread_pool<T>::worker(void *arg)
{
	thread_pool * pool  =(thread_pool *) arg;
	pool->run( );
	return pool;
}
template<typename T>
bool thread_pool<T>::append(T * request)
{
	m_queuelock.lock();
	if( m_workQueue.size()>m_max_request)
	{
		m_workQueue.unlock();
		return false;
	}
	m_workQueue.push_back(request);
	m_queuelock.unlock();
	m_queuestat.post( );
	return true;

}

template<typename T>
 thread_pool<T>:: thread_pool(connection_pool * connPool,int thread_number,int max_request):m_thread_number(thread_number),
 m_max_request(max_request),m_conn_pool(connection_pool),m_stop(false)
{
   	if(thread_number<0||max_request<=0)
       		throw std::exception();
    	m_threads=new pthread_t[m_thread_number];
    	//动态数组
	if(m_threads)
	{
		throw std::exception();
	}
	
	/* 完成对线程池的初始化, space ~ time */
	for(int i=0;i<m_thread_number)
	{
		if(pthread_create(m_threads+i,NULL,worker,this)!=0)
		{
			delete [] m_threads;
			throw std::exception();
		}
		if(pthread_detach(m_threads[i]))
		{
			delete[ ] m_threads;
			throw std::exception();
		}

	}




}

 thread_pool<T>::~ thread_pool()
{
	delete [ ] m_threads;
	m_stop=true;
}






















#endif