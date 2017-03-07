
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

#endif