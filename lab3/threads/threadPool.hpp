
#ifndef THREAD_POOL_HPP
#define THREAD_POOL_HPP

/*************************************\

 threadPool.hpp
 Author: Michael Laucella
 Last Modified: 3/8/17

 skeleton for threadPool class

 meant for multithreading using classes
 
\*************************************/

#include <pthread.h>

#define THREADS 4 //default number of threads

class ThreadPool {
    pthread_t *threads;
    long numThreads;
public:
    //constructors
    ThreadPool();
    ThreadPool(long n);
    ~ThreadPool();
    void createThreads();                       //create the threads
    static void* classThread(void* instance);   //give the thread class capability and access to (this) 
protected:
    virtual void* run();                        //overidable method for threads to run
};

#endif