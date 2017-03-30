
/*************************************\

 threadPool.cpp
 Author: Michael Laucella
 Last Modified: 3/8/17

 implements the threadPool class in
 threadPool.hpp

 meant for extension and multithreading
 using classes

 current run function does nothing
 override method and create new 
 definition to use class properly

\*************************************/

#include "threadPool.hpp"

//constructor
ThreadPool::ThreadPool(): numThreads(THREADS){
    threads=new pthread_t[numThreads];
}

//constructor
ThreadPool::ThreadPool(long n): numThreads(n){
    threads=new pthread_t[numThreads];
}

//destructor
ThreadPool::~ThreadPool(){
    for(long i=0; i<numThreads; i++){
        pthread_join(threads[i], NULL);
    }
    delete threads;
}

//create the threads
void ThreadPool::createThreads(){
    int created;
    int failed=0;

    //create the threads ignore but keep note of any failed thread creations
    for(long i=0; i<numThreads; i++){
        created=pthread_create(&threads[i], NULL, classThread, this);
        if(created)
            failed++;
    }

    numThreads-=failed;
}

//default task to run overidable
void* ThreadPool::run(){
    pthread_exit(NULL);
}

//use class casting to allow the addition of this to static function 
void* ThreadPool::classThread(void* instance){
    //uses pointer to instance of itself to run the function
    ((ThreadPool*) instance)->run();
    return NULL;
}