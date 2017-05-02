
#pragma once

/*************************************\

 threadPoolServer.hpp
 Author: Michael Laucella
 Last Modified: 4/22/17

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
#include "../files/filesys.hpp"
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

const size_t CACHE_SIZE=150;

#ifdef CACHE_LIST
#include "../datastructs/caches/lruCache.hpp"
typedef LRUCache<std::string, std::string> Cache;
#else
#include "../datastructs/caches/lruCacheMap.hpp"
typedef LRUCacheMap<std::string, std::string> Cache;
#endif

#ifndef LFREE
#include "../datastructs/queue.h"
typedef ThreadSafeListenerQueue<Data*> Queue;
#else
#include "../datastructs/lockFreeQueue.hpp"
typedef LockFreeQueue<Data*> Queue;
#endif

//#define ALGO_PATH ""
#ifdef NO_CACHE
#define ALGO_PATH "/NoCache"
#else
#define ALGO_PATH "/Cache"
#endif

class ThreadPoolServer : public ThreadPool, public Server{
    ThreadSafeKVStore<std::string, std::string> hashes;
    ThreadSafeFiles files;
    Cache cache;
    Queue queue;
    std::ofstream fd;
    std::mutex timeLock;
    int numThreads;
public:
    ThreadPoolServer(int threads, int port, int listen);    //constructor
    void* run();         //thread run function
    void handleConn();   //connection handling
    void handle_shutdown();
    static void shutdown(int sig);
};

static ThreadPoolServer* instance;
