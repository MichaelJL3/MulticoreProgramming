
#ifndef THREAD_POOL_SERVER_HPP
#define THREAD_POOL_SERVER_HPP

/*************************************\

 threadPoolServer.hpp
 Author: Michael Laucella
 Last Modified: 3/8/17

 skeleton for mulithreaded tcp/ip server

 extends both the threadpool and the Server
 class

 used for a very specific purpose in 
 creating a kv server store

 uses crypto++ for md5 key hashing
 and provided httpreq/httpresp for
 http parsing

 uses a queue listener for message
 passing between threadpool and 
 server

\*************************************/

#include "threadPool.hpp"
#include "../datastructs/hashmap.h"
#include "../network/server.hpp"
#include "../httpreq/httpreq.hpp"
#include "../httpreq/httpresp.hpp"
#include "../hashing/md5.hpp"
#include "../datastructs/caches/lruCache.hpp"
#include "../files/filesys.hpp"

#define PATH ""
#define ALGO_PATH ""

#ifdef STATS
#ifndef BIGLOCK
#define PATH "/ReadWriteLock"
#else
#define PATH "/GlobalLock"
#endif
#endif

#ifndef LFREE
#ifdef STATS
#define ALGO_PATH "/RegQueue"
#endif
#include "../datastructs/queue.h"
#else
#ifdef STATS
#define ALGO_PATH "/LockFree"
#endif
#include "../datastructs/lockFreeQueue.hpp"
#endif

#ifdef STATS
#include <iostream>
#include <signal.h>
#include <mutex>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <chrono>
#include <ctime>

struct Data{
    int conn;
    std::chrono::high_resolution_clock::time_point st;
};

static bool sortLessThan(double lft, double rht){
   return lft < rht;
}

#endif

const size_t CACHE_SIZE=150;

class ThreadPoolServer : public ThreadPool, public Server{
    ThreadSafeKVStore<std::string, std::string> hashes;
    LRUCache<std::string, std::string> cache;

    #ifndef LFREE
    #ifndef STATS
    ThreadSafeListenerQueue<int> queue;
    #else
    ThreadSafeListenerQueue<Data*> queue;
    #endif
    #else
    #ifndef STATS
    LockFreeQueue<int> queue;
    #else
    LockFreeQueue<Data*> queue;
    #endif
    #endif
    
    #ifdef STATS
    std::ofstream fd;
    int _gets;
    int _posts;
    int _deletes;
    std::vector<double> times;
    std::mutex timeLock;
    double avg;
    double min;
    double max;
    double med;
    #endif
    
    int numThreads;
public:
    ThreadPoolServer(int threads, int port, int listen);    //constructor
    void* run();         //thread run function
    void handleConn();   //connection handling
    void handle_shutdown();
    static void shutdown(int sig);
};

#ifdef STATS
static ThreadPoolServer* instance;
#endif

#endif
