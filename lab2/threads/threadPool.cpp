
#ifdef THREAD_POOL_HPP

ThreadPool::ThreadPool(){
    numThreads=THREADS;
    threads=new pthread_t[numThreads];
}

ThreadPool::ThreadPool(long n){
    numThreads=n;
    threads=new pthread_t[numThreads];
}

ThreadPool::~ThreadPool(){
    for(long i=0; i<numThreads; i++){
        pthread_join(threads[i], NULL);
    }
    delete threads;
}

void ThreadPool::createThreads(){
    int created;
    int failed=0;

    for(long i=0; i<numThreads; i++){
        created=pthread_create(&threads[i], NULL, classThread, this);
        if(created)
            failed++;
    }

    numThreads-=failed;
}

void* ThreadPool::run(){
    pthread_exit(NULL);
}

void* ThreadPool::classThread(void* instance){
    ((ThreadPool*) instance)->run();
    return NULL;
}

#endif