
#ifndef THREAD_POOL_SERVER_HPP
#define THREAD_POOL_SERVER_HPP

#include "threadPool.hpp"

class ThreadPoolServer : public ThreadPool{
    ThreadSafeKVStore<std::string, int32_t> hmap;
	ThreadSafeListenerQueue<int32_t> queue;
    HTTPReq request;
public:
    void* run();
    void getRequest();
    void postRequest();
    void deleteRequest();
};

getRequest(){
    std::string key=request.getURI();

    //key=md5(key.substr(1));

    if(hmap.find(key)==-1){
        //return 404
    }

    return hmap[key];
}

#include "threadPoolServer.cpp" 

#endif