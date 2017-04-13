
#include "threadPoolServer.hpp" 

/*************************************\

 threadPoolServer.cpp
 Author: Michael Laucella
 Last Modified: 3/8/17

 implements the threadPoolServer class in
 threadPoolServer.hpp

 extends both the threadpool and the Server
 class to allow for a multithreaded server

 used for a very specific purpose in 
 creating a kv server store

 uses crypto++ for md5 key hashing
 and provided httpreq/httpresp for
 http parsing

 uses a queue listener for message
 passing between threadpool and 
 server

\*************************************/

//constructor
ThreadPoolServer::ThreadPoolServer(int threads, int port, int listen=1024) : ThreadPool(threads), Server(1024, listen, port) {
    #ifdef STATS
    signal(SIGINT, shutdown);
    fd.open ("analyze/stats.csv", std::ofstream::out | std::ofstream::app);
    #endif
}

void ThreadPoolServer::shutdown(int sig){
	#ifdef STATS
    std::cout<<"\nShuttingdown: ("<<sig<<")\nGets: "<<_gets<<"\nPosts: "<<_posts<<"\nDeletes: "<<_deletes<<"\n";
	#endif

    exit(sig);
}

//thread run function
#ifndef STATS
void* ThreadPoolServer::run(){
    std::string reqType, key, val, body="";
    MD5Hash md5;
    int conn, code;
    bool alive;

    while(true){
        //wait for connection
        queue.listen(conn);

        HTTPReq req(conn);
	    code=404;

        //ignore malformed or failed parses as 404
        if(req.parse()!=-1||!req.isMalformed()){
            reqType=req.getMethod();
            key=md5.getHash(req.getURI());

            if(reqType=="GET"){
                code=(hmap.lookup(key, body)==-1?404:200);
            }
            else if(reqType=="POST"){
                val=req.getBody();
                hmap.insert(key, val);
                code=200;
            }
            else if(reqType=="DELETE"){
                if(hmap.lookup(key, val)==-1)
                    code=404;
                else{
                    hmap.remove(key);
                    code=200;
                }
            }
        }

        //check for keep alive connection
        alive=req.keepAlive();
        
        //sedn response
        HTTPResp res(code, body, alive);
        send(res.isMalformed()?"HTTP/1.1 501 Internal Server Error\r\n\r\n":res.getResponse(), conn);

        //push back on queue or close connection
        if(alive)
            queue.push(conn);
        else
            closeConn(conn);
        
        //optional logging
        #ifdef INFO 
        LOG("\nREQ: "<<req<<"\nKEEP-ALIVE: "<<alive<<"\nRES: "<<res.getResponse()<<"\nKEY: "<<key);
        #endif
        
        //clear out the md5 buffer
        md5.clear();
    }

    pthread_exit(NULL);
}

#else
void* ThreadPoolServer::run(){
    std::string reqType, key, val, body="";
    MD5Hash md5;
    int conn, code;
    bool alive;

    Data *dt;
    clock_t en_time, st_time;

    while(true){

        //wait for connection
        queue.listen(dt);
        conn=dt->conn;
        st_time=dt->st;

        HTTPReq req(conn);
	    code=404;

        //ignore malformed or failed parses as 404
        if(req.parse()!=-1||!req.isMalformed()){
            reqType=req.getMethod();
            key=md5.getHash(req.getURI());

            if(reqType=="GET"){
                std::atomic_fetch_add(&_gets, 1);
                code=(hmap.lookup(key, body)==-1?404:200);
            }
            else if(reqType=="POST"){
                std::atomic_fetch_add(&_posts, 1);
                val=req.getBody();
                hmap.insert(key, val);
                code=200;
            }
            else if(reqType=="DELETE"){
                std::atomic_fetch_add(&_deletes, 1);
                if(hmap.lookup(key, val)==-1)
                    code=404;
                else{
                    hmap.remove(key);
                    code=200;
                }
            }
        }

        //check for keep alive connection
        alive=req.keepAlive();
        
        //sedn response
        HTTPResp res(code, body, alive);
        send(res.isMalformed()?"HTTP/1.1 501 Internal Server Error\r\n\r\n":res.getResponse(), conn);

        //push back on queue or close connection
        dt->st=clock();
        if(alive)
            queue.push(dt);
        else{
            delete dt;
            closeConn(conn);
        }

        en_time=clock();
        timeLock.lock();
        fd<<reqType+","+std::to_string(((double)en_time-st_time)/CLOCKS_PER_SEC)+"\n";
        fd.flush();
        timeLock.unlock();

        //optional logging
        #ifdef INFO 
        LOG("\nREQ: "<<req<<"\nKEEP-ALIVE: "<<alive<<"\nRES: "<<res.getResponse()<<"\nKEY: "<<key);
        #endif
        
        //clear out the md5 buffer
        md5.clear();
    }

    pthread_exit(NULL);
}
#endif

//handle incoming connections
void ThreadPoolServer::handleConn(){
    #ifndef STATS
    queue.push(this->getConn());
    #else
    Data *dt=new Data{this->getConn(), clock()};
    queue.push(dt);
    #endif
}
