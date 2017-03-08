
#ifdef THREAD_POOL_SERVER_HPP

void* ThreadPoolServer::run(){
    //loop forever listen on queue
    /*
    type request;

    while(true){
        queue.listen(request);

        //process request

        //send results
    }
    */

    pthread_exit(NULL);
}

void ThreadPoolServer::handleConn(){
    std::string reqType;

    recv();
    HTTPReq req(this->getMsg(), this->getMsgSize());

    if(req.parse()==-1||req.isMalformed()){
        send("invalid\n");
    }else{
        reqType=req.getMethod();
        send("valid\n");
        if(reqType=="GET"){
            send("get\n");
        }else if(reqType=="POST"){
            send("post\n");
        }else if(reqType=="DELETE"){
            send("del\n");
        }else{
            send("err\n");
        }
    }

    closeConn();
}

#endif