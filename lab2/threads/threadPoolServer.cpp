
#ifdef THREAD_POOL_SERVER_HPP

ThreadPoolServer::ThreadPoolServer(int threads, int port) : ThreadPool(threads), Server(1024, threads, port){

}

//Switch to new httpreq class
//implement loadbalancing on multi request keep-alive
//reenable keepalive
//comment out
//add in some informational debugging

void* ThreadPoolServer::run(){
    Container* data;
    std::string reqType, key, val, body="";
    MD5Hash md5;
    int conn, code=404;

    while(true){
        queue.listen(data);

        conn=data->conn;
        HTTPReq req(data->message, data->msgSize);
        delete data;

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

        HTTPResp res(code, body, false);
        send(res.isMalformed()?"HTTP/1.1 501 Internal Server Error\r\n\r\n":res.getResponse(), conn);
            
        closeConn(conn);
        md5.clear();
    }

    pthread_exit(NULL);
}

void ThreadPoolServer::handleConn(){
    recv();
    if(this->getMsgSize()){
        Container* data=new Container(this->getConn(), this->getMsg(), this->getMsgSize());
        queue.push(data);
    }else{
        closeConn(this->getConn());
    }
}

#endif