
#include "threadPoolServer.hpp" 

/*************************************\

 threadPoolServer.cpp
 Author: Michael Laucella
 Last Modified: 4/22/17

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
ThreadPoolServer::ThreadPoolServer(int threads, int port, int listen=1024) : ThreadPool(threads), Server(1024, listen, port), files("stored/"), cache(CACHE_SIZE) {
    numThreads=threads;

    instance=this;
    signal(SIGINT, ThreadPoolServer::shutdown);
    fd.open ("analyze/"+std::to_string(threads)+ALGO_PATH+"/stats.csv", std::ofstream::out);
    if(!fd.is_open())
        std::cout<<"Failed to open stats file: " << std::to_string(threads) << ALGO_PATH << "/stats.csv\n";

    #ifdef NO_CACHE
    std::cout<<"Cache: Disabled\n";
    #else
    std::cout<<"Cache: Enabled\nCache Size: "<< CACHE_SIZE << "\n";
    #endif

    #ifdef LFREE
    std::cout<<"Lock Free Queue: Enabled\n";
    #else
    std::cout<<"Lock Free Queue: Disabled\n";
    #endif 

    #ifdef INFO
    std::cout<<"Addtnl Info: Enabled\n";
    #else
    std::cout<<"Addtnl Info: Disabled\n";
    #endif
}

void ThreadPoolServer::handle_shutdown(){
    size_t pos;
    double avg=0;
    double min=0;
    double max=0;
    double med=0;
    int _gets=0;
    int _posts=0;
    int _deletes=0;

    fd.flush();
    fd.close();
    
    std::vector<double> times;
    std::ifstream stats ("analyze/"+std::to_string(numThreads)+ALGO_PATH+"/stats.csv");
    std::string line;
    std::string type;    

    if(stats.is_open()){
        while( getline(stats,line) ){
            pos=line.find(",");

            if(pos!=std::string::npos){
                type=line.substr(0,pos);
                if(type=="GET")
                    ++_gets;
                else if(type=="POST")
                    ++_posts;
                else if(type=="DELETE")
                    ++_deletes;

                line=line.substr(pos+1);
                times.push_back(std::stod(line));
            }
        }
    }else{
        std::cout<<"Failed to open stats file: " << std::to_string(numThreads) << ALGO_PATH << "/stats.csv\n";
    }
    stats.close();
    
    std::sort(times.begin(), times.end(), sortLessThan);

    if(times.size()){
         for(auto time:times){
             avg+=time;
         }
   
         avg/=times.size();
         max=times[times.size()-1];
         min=times[0];

         if(!(times.size()%2))
              med=times[times.size()/2+(times.size()+1)/2];
         else
              med=times[times.size()/2];
    }

    std::cout<<"\nShuttingdown:\nGets: "<<_gets<<"\nPosts: "<<_posts<<"\nDeletes: "<<_deletes<<"\n";
    std::cout<<"Stats\nAvg: "<<avg<<"\nMin: "<<min<<"\nMax: "<<max<<"\nMedian: "<<med<<"\n";
}

void ThreadPoolServer::shutdown(int sig){
    instance->handle_shutdown();
    exit(sig);
}

void* ThreadPoolServer::run(){
    std::string reqType, key, user, body="";
    MD5Hash md5;
    int conn, code;
    bool alive;

    Data *dt;
    std::chrono::high_resolution_clock::time_point st, en;

    while(true){

        //wait for connection
        queue.listen(dt);
        conn=dt->conn;
        st=dt->st;

        HTTPReq req(conn);
        code=404;

        //ignore malformed or failed parses as 404
        if(req.parse()!=-1||!req.isMalformed()){
            reqType=req.getMethod();

            //check if key-md5 is cached
            user=req.getURI();
            if(hashes.lookup(user, key)==-1){
                key=md5.getHash(user);
                hashes.insert(user, key);
            }

            #ifndef NO_CACHE
            if(reqType=="GET")
                code=(cache.get(key, body)||files.rdFile(user, body)?200:404);
            else if(reqType=="POST"){
                body=req.getBody();
                cache.insert(key, body);
                code=(files.wrFile(user, body)?200:500);
            }
            else if(reqType=="DELETE")
                code=(cache.erase(key)||files.dlFile(user)?200:404);
            #else
            if(reqType=="GET")
                code=(files.rdFile(user, body)?200:404);
            else if(reqType=="POST"){
                body=req.getBody();
                code=(files.wrFile(user, body)?200:500);
            }
            else if(reqType=="DELETE")
                code=(files.dlFile(user)?200:404);
            #endif
        }

        //check for keep alive connection
        alive=req.keepAlive();
        
        //sedn response
        HTTPResp res(code, body, alive);
        send(res.isMalformed()?"HTTP/1.1 501 Internal Server Error\r\n\r\n":res.getResponse(), conn);

        //push back on queue or close connection
        if(alive){
            dt->st=std::chrono::high_resolution_clock::now();
            queue.push(dt);
        }else{
            delete dt;
            closeConn(conn);
        }

        en=std::chrono::high_resolution_clock::now();
        timeLock.lock();
        fd<<reqType+","+std::to_string(std::chrono::duration_cast<std::chrono::microseconds>(en-st).count())<<"\n";
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

//handle incoming connections
void ThreadPoolServer::handleConn(){
    Data *dt=new Data{this->getConn(), std::chrono::high_resolution_clock::now()};
    queue.push(dt);
}
