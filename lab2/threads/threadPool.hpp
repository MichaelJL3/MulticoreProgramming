
#ifndef THREAD_POOL_HPP
#define THREAD_POOL_HPP

#include <pthread.h>

#define THREADS 4			//default number of threads

class ThreadPool {
    pthread_t *threads;
    long numThreads;
public:
    ThreadPool();
    ThreadPool(long n);
    ~ThreadPool();
    void createThreads();
    static void* classThread(void* instance);
protected:
    virtual void* run();
};

#include "threadPool.cpp"

#endif