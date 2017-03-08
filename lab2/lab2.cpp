
#include "lab2.h"

#include <iostream>
#include "threads/threadPoolServer.hpp"

#include "hashing/md5.hpp"

using namespace std;

int main(){

    ThreadPoolServer server;
    server.start();

    //implement the MD5

    //ThreadPool pool(4);

    /*HTTPReq req("GET / HTTP/1.1\r\n", 17);

    if(req.parse()==-1||req.isMalformed()){
        //errors!
    }

    switch(req.getMethod()){
        case "GET": 
            break;
        case "POST": 
            break;
        case "DELETE": 
            break;
        default: 
            break;
    }

    cout<<req.getURI()<<endl;
    cout<<req.getBody()<<endl;*/

    return 0;
}