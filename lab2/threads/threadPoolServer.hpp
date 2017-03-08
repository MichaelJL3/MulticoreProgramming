
#ifndef THREAD_POOL_SERVER_HPP
#define THREAD_POOL_SERVER_HPP

#include "threadPool.hpp"

class ThreadPoolServer : public ThreadPool, public Server{
    ThreadSafeKVStore<std::string, int32_t> hmap;
	ThreadSafeListenerQueue<int32_t> queue;
public:
    void* run();
    //void handleConn();
};

#include "threadPoolServer.cpp" 

#endif