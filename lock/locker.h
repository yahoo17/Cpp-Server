#ifndef LOCKER_H
#define LOCKER_H

#include <list>
#include <stdio.h>
#include <pthread.h>
#include<semaphore.h>
template<typename T>
class sem
{
public:
        sem()
        {
                if(sem_init(&m_sem,0,0)!=0)
                {
                        throw std::exception();
                }
        }
        sem(int num)
        {
                if(sem_init(&m_sem,0,num)!=0)
                        throw std::exception();
        }
        bool wait()
        {
                return sem_wait(&m_sem)==0;

        }
        bool post()
        {
                return sem_post(&m_sem)==0;
        }
        ~sem()
        {
                sem_destroy(&m_sem);

        }
private:
        sem_t m_sem;
};
class cond
{
private:
        /* data */
        pthread_cond_t   m_cond;
        pthread_mutex_t m_mutex;
public:
        cond(/* args */);
        ~cond();
        bool wait(pthread_mutex_t * m_mutex) 
        {
                int ret=0;
                ret=pthread_cond_wait(&m_cond,m_mutex);
                return ret==0;
        };
        bool timewait(pthread_mutex_t *m_mutex,struct timespec t)
        {
                int ret=0;
                ret=pthread_cond_timedwait(& m_cond,m_mutex,&t);
                return ret==0;
        }
        bool signal()
        {
                return pthread_cond_signal(&m_cond)==0;
        }
        bool broadcast()
        {
                return pthread_cond_broadcast(&m_cond)==0;
        }
        
        
};

cond::cond(/* args */)
{
        if(pthread_cond_init(&m_cond ,NULL)!=0)
        {
                throw std::exception();
        }
}

cond::~cond()
{
        pthread_cond_destroy(&m_cond);
}

class locker
{
private:
        pthread_mutex_t m_mutex;
public:
        locker(/* args */);
        ~locker();
        bool lock();
        bool unlock();
};
bool locker::unlock()
{
        return pthread_mutex_unlock(&m_mutex);
}
bool locker::lock()
{
        return pthread_mutex_lock(&m_mutex);

}
locker::locker(/* args */)
{
        if(pthread_mutex_init(&m_mutex,NULL)!=0)
        {
                throw std::exception();
        }
}

locker::~locker()
{
        pthread_mutex_destroy(&m_mutex);
}












#endif